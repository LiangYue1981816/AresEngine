#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxMaterial.h"


CGfxMaterial::CGfxMaterial(uint32_t name)
	: m_name(name)
{

}

CGfxMaterial::~CGfxMaterial(void)
{
	Destroy();
}

void CGfxMaterial::Release(void)
{
	Renderer()->DestroyMaterial(this);
}

uint32_t CGfxMaterial::GetName(void) const
{
	return m_name;
}

bool CGfxMaterial::Load(const char *szFileName)
{
	/*
	<Material>
		<Pass name="">
			<Pipeline>
				<Vertex file_name="">
					<Define name="" />
					<Define name="" />
				</Vertex>
				<Fragment file_name="">
					<Define name="" />
					<Define name="" />
				</Fragment>
				<State>
					<Cull enable="" cull_face="" front_face="" />
					<Depth enable_test="" enable_write="" depth_func="" />
					<Color enable_write_red="" enable_write_green="" enable_write_blue="" enable_write_alpha="" />
					<Blend enable="" src_factor="" dst_factor="" />
					<Offset enable="" factor="" units="" />
				</State>
			</Pipeline>

			<Texture2D file_name="" name="" min_filter="" mag_filter="" address_mode="" />
			<Texture2DArray file_name="" name="" min_filter="" mag_filter="" address_mode="" />
			<TextureCubeMap file_name="" name="" min_filter="" mag_filter="" address_mode="" />

			<Uniform1f name="" value="" />
			<Uniform2f name="" value="" />
			<Uniform3f name="" value="" />
			<Uniform4f name="" value="" />
		</Pass>
	</Material>
	*/

	try {
		Destroy();

		LogOutput("GfxRenderer", "LoadMaterial(%s)\n", szFileName);
		{
			TiXmlDocument xmlDoc;
			if (xmlDoc.LoadFile(Renderer()->GetResourceFullName(szFileName)) == false) throw 0;

			TiXmlNode *pMaterialNode = xmlDoc.FirstChild("Material");
			if (pMaterialNode == NULL) throw 1;

			if (TiXmlNode *pPassNode = pMaterialNode->FirstChild("Pass")) {
				do {
					if (LoadPass(pPassNode) == false) {
						throw 2;
					}
				} while (pPassNode = pMaterialNode->IterateChildren("Pass", pPassNode));
			}
		}
		LogOutput("GfxRenderer", "OK\n");
		return true;
	}
	catch (int err) {
		Destroy();
		LogOutput("GfxRenderer", "Fail(%d)\n", err);
		return false;
	}
}

bool CGfxMaterial::LoadPass(TiXmlNode *pPassNode)
{
	uint32_t name = HashValue(pPassNode->ToElement()->AttributeString("name"));

	if (m_pPasses[name] == NULL) {
		m_pPasses[name] = new CGfxMaterialPass(name);
		return m_pPasses[name]->Load(pPassNode);
	}

	return true;
}

void CGfxMaterial::Destroy(void)
{
	for (const auto &itPass : m_pPasses) {
		delete itPass.second;
	}

	m_pPasses.clear();
}

const CGfxMaterialPass* CGfxMaterial::GetPass(uint32_t name) const
{
	const auto &itPass = m_pPasses.find(name);
	return itPass != m_pPasses.end() ? itPass->second : NULL;
}
