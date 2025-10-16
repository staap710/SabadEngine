#pragma once

namespace SabadEngine::Graphics
{
    class ConstantBuffer
    {
    public:
        ConstantBuffer() = default;
        virtual ~ConstantBuffer();

        void Initialize(uint32_t bufferSize);
        void Terminate();

        void Update(const void* data) const;

        void BindVS(uint32_t slot) const;
        void BindPS(uint32_t slot) const;

    private:
        ID3D11Buffer* mConstantBuffer = nullptr;
    };

    template <class DataType>
    class TypedConstantBuffer final : public ConstantBuffer
    {
    public:
        void Initialize()
        {
            static_assert(sizeof(DataType) % 16 == 0, "Data must be 16 bytes aligned!");
            ConstantBuffer::Initialize(sizeof(DataType));

        }
        void Update(const DataType& data) const
        {
            ConstantBuffer::Update(&data);
        }
    };
}