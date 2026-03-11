#pragma once

#ifndef USE_LEGACY_RANDOM
class Random
{
private:
	int64_t seed;                    // last seed used (for reference)
	bool haveNextNextGaussian;       // state for nextGaussian()
	double nextNextGaussian;         // cached Gaussian value

protected:
	int next(int bits);               // generates up to 32 random bits

public:
	Random();                          // uses a high-entropy seed
	explicit Random(int64_t seed);     // seed with a specific value
	void setSeed(int64_t s);           // re-seed the generator

	void nextBytes(byte* bytes, unsigned int count);
	double nextDouble();
	double nextGaussian();
	int nextInt();
	int nextInt(int n);                // in [0, n)
	float nextFloat();
	int64_t nextLong();
	bool nextBoolean();
};
#else
class Random
{
private:
	int64_t seed;
	bool haveNextNextGaussian;
	double nextNextGaussian;
protected:
	int next(int bits);
public:
	Random();
	Random(int64_t seed);
	void setSeed(int64_t s);
	void nextBytes(byte *bytes, unsigned int count);
	double nextDouble();
	double nextGaussian();
	int nextInt();
	int nextInt(int to);
	float nextFloat();
	int64_t nextLong();
	bool nextBoolean();
};
#endif
