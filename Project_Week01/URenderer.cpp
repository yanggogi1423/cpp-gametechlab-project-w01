#include "URenderer.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

void URenderer::Create(HWND hWindow) {
    CreateDeviceAndSwapChain(hWindow);
    CreateFrameBuffer();
    CreateRasterizerState();
}

void URenderer::CreateDeviceAndSwapChain(HWND hWindow) {
    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

    DXGI_SWAP_CHAIN_DESC swapchaindesc = {};
    swapchaindesc.BufferDesc.Width = 0;
    swapchaindesc.BufferDesc.Height = 0;
    swapchaindesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapchaindesc.SampleDesc.Count = 1;
    swapchaindesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchaindesc.BufferCount = 2;
    swapchaindesc.OutputWindow = hWindow;
    swapchaindesc.Windowed = TRUE;
    swapchaindesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

    D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
        //D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT,
        featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION,
        &swapchaindesc, &SwapChain, &Device, nullptr, &DeviceContext
    );

    SwapChain->GetDesc(&swapchaindesc);
    ViewportInfo = { 0.0f, 0.0f, (float)swapchaindesc.BufferDesc.Width, (float)swapchaindesc.BufferDesc.Height, 0.0f, 1.0f };
}

void URenderer::CreateFrameBuffer() {
    SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&FrameBuffer);

    D3D11_RENDER_TARGET_VIEW_DESC framebufferRTVdesc = {};
    framebufferRTVdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
    framebufferRTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

    Device->CreateRenderTargetView(FrameBuffer, &framebufferRTVdesc, &FrameBufferRTV);
}

void URenderer::CreateRasterizerState() {
    D3D11_RASTERIZER_DESC rasterizerdesc = {};
    rasterizerdesc.FillMode = D3D11_FILL_SOLID;
    rasterizerdesc.CullMode = D3D11_CULL_BACK;

    Device->CreateRasterizerState(&rasterizerdesc, &RasterizerState);
}

void URenderer::CreateShader() {
    ID3DBlob* vertexshaderCSO = nullptr;
    ID3DBlob* pixelshaderCSO = nullptr;

    D3DCompileFromFile(L"ShaderW0.hlsl", nullptr, nullptr, "mainVS", "vs_5_0", 0, 0, &vertexshaderCSO, nullptr);
    Device->CreateVertexShader(vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), nullptr, &SimpleVertexShader);

    D3DCompileFromFile(L"ShaderW0.hlsl", nullptr, nullptr, "mainPS", "ps_5_0", 0, 0, &pixelshaderCSO, nullptr);
    Device->CreatePixelShader(pixelshaderCSO->GetBufferPointer(), pixelshaderCSO->GetBufferSize(), nullptr, &SimplePixelShader);

    D3D11_INPUT_ELEMENT_DESC layout[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    Device->CreateInputLayout(layout, ARRAYSIZE(layout), vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), &SimpleInputLayout);

    Stride = sizeof(FVertex);

    if (vertexshaderCSO) vertexshaderCSO->Release();
    if (pixelshaderCSO) pixelshaderCSO->Release();
}

void URenderer::CreateConstantBuffer() {
    D3D11_BUFFER_DESC constantbufferdesc = {};
    // 16바이트 정렬 보정 로직 보강
    constantbufferdesc.ByteWidth = (sizeof(FConstants) + 15) & ~0xF;
    constantbufferdesc.Usage = D3D11_USAGE_DYNAMIC;
    constantbufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    constantbufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    Device->CreateBuffer(&constantbufferdesc, nullptr, &ConstantBuffer);
}

void URenderer::Prepare() {
    DeviceContext->ClearRenderTargetView(FrameBufferRTV, ClearColor);
    DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    DeviceContext->RSSetViewports(1, &ViewportInfo);
    DeviceContext->RSSetState(RasterizerState);
    DeviceContext->OMSetRenderTargets(1, &FrameBufferRTV, nullptr);
}

void URenderer::PrepareShader() {
    DeviceContext->VSSetShader(SimpleVertexShader, nullptr, 0);
    DeviceContext->PSSetShader(SimplePixelShader, nullptr, 0);
    DeviceContext->IASetInputLayout(SimpleInputLayout);

    

    if (ConstantBuffer) {
        DeviceContext->VSSetConstantBuffers(0, 1, &ConstantBuffer);
    }
}

// renderer 는 constant를 update하기만 할 것
void URenderer::UpdateConstant(const DirectX::XMMATRIX pXMMATRIX) {
    if (ConstantBuffer) {
        D3D11_MAPPED_SUBRESOURCE constantbufferMSR;
        if (SUCCEEDED(DeviceContext->Map(ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &constantbufferMSR))) {
            memcpy(constantbufferMSR.pData, &pXMMATRIX, sizeof(DirectX::XMMATRIX));
            DeviceContext->Unmap(ConstantBuffer, 0);
        }
    }
}

//ID3D11Buffer* URenderer::CreateVertexBuffer(FVertex* vertices, UINT bytewidth) {
//    D3D11_BUFFER_DESC vertexbufferdesc = {};
//    vertexbufferdesc.ByteWidth = bytewidth;
//    vertexbufferdesc.Usage = D3D11_USAGE_IMMUTABLE;
//    vertexbufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//
//    D3D11_SUBRESOURCE_DATA vertexbufferSRD = { vertices };
//    ID3D11Buffer* vertexBuffer = nullptr;
//
//    Device->CreateBuffer(&vertexbufferdesc, &vertexbufferSRD, &vertexBuffer);
//    return vertexBuffer;
//}



void URenderer::indexRenderPrimitive(UINT numIndices)
{
    DeviceContext->DrawIndexed(numIndices,0,0);
}




void URenderer::SwapBuffer() {
    SwapChain->Present(1, 0);
}

// --- Release 함수들 ---

void URenderer::Release() {
    if (DeviceContext) DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

    ReleaseConstantBuffer();
    ReleaseShader();
    ReleaseRasterizerState();
    ReleaseFrameBuffer();
    ReleaseDeviceAndSwapChain();
}

void URenderer::ReleaseDeviceAndSwapChain() {
    if (DeviceContext) DeviceContext->Flush();
    if (SwapChain) SwapChain->Release();
    if (Device) Device->Release();
    if (DeviceContext) DeviceContext->Release();

    SwapChain = nullptr;
    Device = nullptr;
    DeviceContext = nullptr;
}

void URenderer::ReleaseFrameBuffer() {
    if (FrameBuffer) FrameBuffer->Release();
    if (FrameBufferRTV) FrameBufferRTV->Release();
    FrameBuffer = nullptr;
    FrameBufferRTV = nullptr;
}

void URenderer::ReleaseShader() {
    if (SimpleInputLayout) SimpleInputLayout->Release();
    if (SimplePixelShader) SimplePixelShader->Release();
    if (SimpleVertexShader) SimpleVertexShader->Release();
    SimpleInputLayout = nullptr;
    SimplePixelShader = nullptr;
    SimpleVertexShader = nullptr;
}

void URenderer::ReleaseRasterizerState() {
    if (RasterizerState) RasterizerState->Release();
    RasterizerState = nullptr;
}



void URenderer::ReleaseVertexBuffer(ID3D11Buffer* vertexBuffer) {
    if (vertexBuffer) vertexBuffer->Release();
}

void URenderer::CreateIndexBuffer(ID3D11Buffer* indexBuffer, UINT* indices, UINT count)
{
    D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    indexBufferDesc.ByteWidth = sizeof(UINT) * count;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = indices;

    Device->CreateBuffer(&indexBufferDesc, &initData, &indexBuffer);

}

void URenderer::ReleaseConstantBuffer() {
    if (ConstantBuffer) ConstantBuffer->Release();
    ConstantBuffer = nullptr;
}

void URenderer::RenderPrimitive(ID3D11Buffer* vertexBuffer, UINT vertexCount) {
    if (!vertexBuffer || !DeviceContext) return;

    UINT offset = 0;
    // 버퍼를 장치에 연결하고 그립니다.
    DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &Stride, &offset);
    DeviceContext->Draw(vertexCount, 0);
}