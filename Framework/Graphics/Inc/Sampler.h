#pragma onve
namespace SabadEngine::Graphics {
	class Sampler
	{
	public:
		enum class Filter{ Point, Linear, Anisotropic};
		enum class AddressMode { Border, Clamp, Mirror, Wrap };

		Samplet() = default;
		~Sampler();
		Sampler(const Sampler&) = delete;
		Sampler& operator=(const Sampler&) = delete;

	private:

	};
}