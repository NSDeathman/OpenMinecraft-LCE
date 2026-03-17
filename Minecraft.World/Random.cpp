#include "stdafx.h"
#include "Random.h"
#include "System.h"

#ifndef USE_LEGACY_RANDOM
// Windows API for high-resolution counters and system info
#include <windows.h>

// C++11 random library and chrono for high-quality seeding
#include <random>
#include <chrono>
#include <thread>

// Use a 64-bit Mersenne Twister engine
static std::mt19937_64& getEngine() {
    // Make the engine thread-local to avoid data races in multi-threaded code
    thread_local std::mt19937_64 engine(std::random_device{}());
    return engine;
}

// ----------------------------------------------------------------------
// Seed generation: combine multiple entropy sources
static int64_t generateEntropySeed()
{
    uint64_t entropy = 0;

    // 1. High-resolution performance counter (similar to original)
    LARGE_INTEGER perfCount;
    QueryPerformanceCounter(&perfCount);
    entropy ^= perfCount.QuadPart;

    // 2. System tick count (milliseconds since boot)
    entropy ^= GetTickCount64();

    // 3. Process and thread IDs
    entropy ^= static_cast<uint64_t>(GetCurrentProcessId()) << 32;
    entropy ^= static_cast<uint64_t>(GetCurrentThreadId()) << 16;

    // 4. High-resolution clock with nanoseconds (C++11)
    auto now = std::chrono::high_resolution_clock::now();
    auto ns = std::chrono::time_point_cast<std::chrono::nanoseconds>(now)
        .time_since_epoch().count();
    entropy ^= static_cast<uint64_t>(ns);

    // 5. Hardware randomness if available (via random_device)
    //    Note: on some compilers random_device may be deterministic,
    //    but it's a good additional source.
    try {
        std::random_device rd;
        entropy ^= static_cast<uint64_t>(rd()) |
            (static_cast<uint64_t>(rd()) << 32);
    }
    catch (...) {
        // random_device not available – just ignore
    }

    // 6. Address of a stack variable (ASLR provides some randomness)
    volatile void* stackAddr = &entropy;
    entropy ^= reinterpret_cast<uintptr_t>(stackAddr);

    // Mix the bits well to avoid correlation
    // (using a simple but effective hash)
    entropy ^= entropy >> 33;
    entropy *= 0xff51afd7ed558ccdULL;
    entropy ^= entropy >> 33;
    entropy *= 0xc4ceb9fe1a85ec53ULL;
    entropy ^= entropy >> 33;

    return static_cast<int64_t>(entropy);
}

// ----------------------------------------------------------------------
// Random class implementation

Random::Random()
{
    // Use the high‑entropy seed generator
    setSeed(generateEntropySeed());
}

Random::Random(int64_t seed)
{
    setSeed(seed);
}

void Random::setSeed(int64_t s)
{
    // Store seed for possible inspection
    seed = s;

    // Seed the Mersenne Twister engine
    getEngine().seed(static_cast<uint64_t>(s));

    // Reset Gaussian cache
    haveNextNextGaussian = false;
    nextNextGaussian = 0.0;
}

// ----------------------------------------------------------------------
// Core bit generator – replaces the old LCG
int Random::next(int bits)
{
    // mt19937_64 produces 64 random bits each call
    uint64_t raw = getEngine()();

    // Return the required number of most significant bits
    // (shifting right keeps the higher bits which are usually "more random")
    return static_cast<int>(raw >> (64 - bits));
}

// ----------------------------------------------------------------------
// All other methods stay exactly as in the original code,
// because they all rely on next(bits).

void Random::nextBytes(byte* bytes, unsigned int count)
{
    for (unsigned int i = 0; i < count; ++i)
    {
        bytes[i] = static_cast<byte>(next(8));
    }
}

double Random::nextDouble()
{
    return ((static_cast<int64_t>(next(26)) << 27) + next(27))
        / static_cast<double>(1LL << 53);
}

double Random::nextGaussian()
{
    if (haveNextNextGaussian)
    {
        haveNextNextGaussian = false;
        return nextNextGaussian;
    }
    else
    {
        double v1, v2, s;
        do
        {
            v1 = 2 * nextDouble() - 1;   // between -1.0 and 1.0
            v2 = 2 * nextDouble() - 1;   // between -1.0 and 1.0
            s = v1 * v1 + v2 * v2;
        } while (s >= 1 || s == 0);
        double multiplier = sqrt(-2 * log(s) / s);
        nextNextGaussian = v2 * multiplier;
        haveNextNextGaussian = true;
        return v1 * multiplier;
    }
}

int Random::nextInt()
{
    return next(32);
}

int Random::nextInt(int n)
{
    // Parameter check (you may replace assert with a thrown exception)
    // assert(n > 0);
    if (n <= 0) return 0;   // or throw std::invalid_argument("n must be positive");

    // Special case for powers of two (fast path)
    if ((n & -n) == n)
        return static_cast<int>((static_cast<int64_t>(next(31)) * n) >> 31);

    int bits, val;
    do
    {
        bits = next(31);
        val = bits % n;
    } while (bits - val + (n - 1) < 0);
    return val;
}

float Random::nextFloat()
{
    return next(24) / static_cast<float>(1 << 24);
}

int64_t Random::nextLong()
{
    return (static_cast<int64_t>(next(32)) << 32) + next(32);
}

bool Random::nextBoolean()
{
    return next(1) != 0;
}
#else
Random::Random()
{
	// 4J - jave now uses the system nanosecond counter added to a "seedUniquifier" to get an initial seed. Our nanosecond timer is actually only millisecond accuate, so
	// use QueryPerformanceCounter here instead
	int64_t seed;
	QueryPerformanceCounter((LARGE_INTEGER *)&seed);
	seed += 8682522807148012LL;

	setSeed(seed);
}

Random::Random(int64_t seed)
{
	setSeed(seed);
}

void Random::setSeed(int64_t s)
{
    this->seed = (s ^ 0x5DEECE66DLL) & ((1LL << 48) - 1);
    haveNextNextGaussian = false;
}

int Random::next(int bits)
{
    seed = (seed * 0x5DEECE66DLL + 0xBLL) & ((1LL << 48) - 1);
    return static_cast<int>(seed >> (48 - bits));
}

void Random::nextBytes(byte *bytes, unsigned int count)
{
	for(unsigned int i = 0; i < count; i++ )
	{
		bytes[i] = static_cast<byte>(next(8));
	}
}

double Random::nextDouble()
{

    return ((static_cast<int64_t>(next(26)) << 27) + next(27))
        / static_cast<double>(1LL << 53);
}

double Random::nextGaussian()
{
    if (haveNextNextGaussian)
	{
        haveNextNextGaussian = false;
        return nextNextGaussian;
    }
	else
	{
        double v1, v2, s;
        do
		{
            v1 = 2 * nextDouble() - 1;   // between -1.0 and 1.0
            v2 = 2 * nextDouble() - 1;   // between -1.0 and 1.0
            s = v1 * v1 + v2 * v2;
        } while (s >= 1 || s == 0);
        double multiplier = sqrt(-2 * log(s)/s);
        nextNextGaussian = v2 * multiplier;
        haveNextNextGaussian = true;
        return v1 * multiplier;
    }
}

int Random::nextInt()
{
	return next(32);
}

int Random::nextInt(int n)
{
    assert (n>0);


    if ((n & -n) == n)  // i.e., n is a power of 2
        return static_cast<int>((static_cast<int64_t>(next(31)) * n) >> 31); // 4J Stu - Made int64_t instead of long

    int bits, val;
    do
	{
        bits = next(31);
        val = bits % n;
    } while(bits - val + (n-1) < 0);
    return val;
}

float Random::nextFloat()
{
	return next(24) / static_cast<float>(1 << 24);
}

int64_t Random::nextLong()
{
	return (static_cast<int64_t>(next(32)) << 32) + next(32);
}

bool Random::nextBoolean()
{
	return next(1) != 0;
}
#endif

bool Random::percent(int percent)
{
    if (percent <= 0) return false;
    if (percent >= 100) return true;
    return nextInt(100) < percent;
}

bool Random::chance(int numerator, int denominator)
{
    if (numerator <= 0) return false;
    if (numerator >= denominator) return true;
    return nextInt(denominator) < numerator;
}
