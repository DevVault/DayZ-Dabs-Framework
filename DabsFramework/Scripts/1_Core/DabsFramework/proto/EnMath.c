modded class Math
{
	// Modulo for floating point numbers
    static float FMod(float num, float divisor)
	{
		return num - (Floor(num / divisor) * divisor);
	}
	
	static float SmoothLerp(float a, float b, float t)
	{
		return (b - a) * SmoothTime(t) + a;
	}
	
	static float SmoothTime(float t)
	{
		return (-Math.Cos(Math.PI * t) / 2) + 0.5;
	}

	static vector LerpVector(vector p1, vector p2, float time)
	{
		return Vector(Math.Lerp(p1[0], p2[0], time), Math.Lerp(p1[1], p2[1], time), Math.Lerp(p1[2], p2[2], time));
	}
	
	static vector SmoothLerpVector(vector p1, vector p2, float time)
	{
		return Vector(Math.SmoothLerp(p1[0], p2[0], time), Math.SmoothLerp(p1[1], p2[1], time), Math.SmoothLerp(p1[2], p2[2], time));
	}
}