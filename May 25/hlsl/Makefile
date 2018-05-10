all: ps.cso vs.cso
	
ps.cso: LightShader.hlsl
	fxc //T ps_5_0 //E "PS" //Fo ps.cso LightShader.hlsl

vs.cso: LightShader.hlsl
	fxc //T vs_5_0 //E "VS" //Fo vs.cso LightShader.hlsl