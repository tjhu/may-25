all: ps.cso vs.cso ps_shadow.cso vs_shadow.cso
	
ps.cso: LightShader.hlsl
	fxc //T ps_5_0 //E "PS" //Fo ps.cso LightShader.hlsl

vs.cso: LightShader.hlsl
	fxc //T vs_5_0 //E "VS" //Fo vs.cso LightShader.hlsl

ps_shadow.cso: ShadowmapShader.hlsl
	fxc //T ps_5_0 //E "PS" //Fo ps_shadow.cso ShadowmapShader.hlsl

vs_shadow.cso: ShadowmapShader.hlsl
	fxc //T vs_5_0 //E "VS" //Fo vs_shadow.cso ShadowmapShader.hlsl