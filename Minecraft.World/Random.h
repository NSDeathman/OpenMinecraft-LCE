#pragma once

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

	// Returns true with probality = percent/100 (percent from 0 to 100)
	bool percent(int percent);

	// Returns true with probality = numerator/denominator
	bool chance(int numerator, int denominator);
};
