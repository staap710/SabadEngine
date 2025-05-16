using namespace SabadEngine::Graphics;

namespace {
	D3D11_FILTER GetFilter(Sampler::Filter filter)
	{
		switch (filter)
		{
		case Sampler::Filter::Point:		return D3D11_FILTER_MIN_MAG_MIP_POINT;
		case Sampler::Filter::Linear:		return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		case Sampler::Filter::Anisotropic:	return D3D11_FILTER_ANISOTROPIC;
		default:
			ASSERT(false, "Filter: missing filter type");
			break;
		}

		return D3D11_FILTER_MIN_MAG_IP_POINT;
	}
	D3D11_TEXTURE_ADDRESS_MODE GetAddressMode(Sampler::AddressMode addressMode)
	{
		switch (addressMode)
		{
		case Sampler::AddressMode::Border:	return D3D11_TEXTURE_ADDRESS_BORDER;
		case Sampler::AddressMode::Clamp:	return D3D11_TEXTURE_ADDRESS_CLAMP;
		case Sampler::AddressMode::Mirror:	return D3D11_TEXTURE_ADDRESS_MIRROR;
		case Sampler::AddressMode::Wrap:		return D3D11_TEXTURE_ADDRESS_WRAP;
		default:
			ASSERT(false, "Address mode not supported");
			break;
		}
		return D3D11_TEXTURE_ADDRESS_BORDER;
	}
}

Sampler::~Sampler() {
	ASSERT(mSampler == nullptr, "Sampler: terminate must be called");
}
void Sampler::Initialize(Filter filter, AddressMode addressMode)
{
	auto d3dFilter = GetFilter(filter);

}