#include "GLES3Renderer.h"
#include "GLES3Material.h"


CGLES3Material::CGLES3Material(CGLES3MaterialManager *pManager, uint32_t name)
	: CGfxMaterial(name)
	, m_pManager(pManager)
{

}

CGLES3Material::~CGLES3Material(void)
{
	Destroy();
}

void CGLES3Material::Release(void)
{
	m_pManager->DestroyMaterial(this);
}

bool CGLES3Material::Load(const char *szFileName)
{
	//<Material>
	//	<Pass name="">
	//		<Pipeline>
	//			<Vertex file_name="">
	//				<Define name="" />
	//				<Define name="" />
	//			</Vertex>
	//			<Fragment file_name="">
	//				<Define name="" />
	//				<Define name="" />
	//			</Fragment>
	//			<State>
	//				<Cull enable="" cull_face="" front_face="" />
	//				<Stencil enable="" func="" ref="" mask="" sfail="" dfail="" dpass="" />
	//				<Depth enable_test="" enable_write="" func="" range_near="" range_far="" />
	//				<Color enable_write_red="" enable_write_green="" enable_write_blue="" enable_write_alpha="" />
	//				<AlphaToCoverage enable="" />
	//				<Blend enable="" src_factor="" dst_factor="" equation="" />
	//				<Offset enable="" factor="" units="" />
	//			</State>
	//		</Pipeline>
	//		<Texture2D file_name="" name="" min_filter="" mag_filter="" address_mode="" />
	//		<Texture2DArray file_name="" name="" min_filter="" mag_filter="" address_mode="" />
	//		<TextureCubeMap file_name="" name="" min_filter="" mag_filter="" address_mode="" />
	//		<Uniform1f name="" value="" />
	//		<Uniform2f name="" value="" />
	//		<Uniform3f name="" value="" />
	//		<Uniform4f name="" value="" />
	//	</Pass>
	//</Material>

	try {
		Destroy();

		LogOutput(LOG_TAG_RENDERER, "LoadMaterial(%s)\n", szFileName);
		{
			CStream stream;
			if (FileManager()->LoadStream(szFileName, &stream) == false) throw 0;

			TiXmlDocument xmlDoc;
			if (xmlDoc.LoadFile((char *)stream.GetAddress(), stream.GetFullSize()) == false) throw 1;

			TiXmlNode *pMaterialNode = xmlDoc.FirstChild("Material");
			if (pMaterialNode == nullptr) throw 2;

			if (TiXmlNode *pPassNode = pMaterialNode->FirstChild("Pass")) {
				do {
					if (LoadPass(pPassNode) == false) {
						throw 3;
					}
				} while (pPassNode = pMaterialNode->IterateChildren("Pass", pPassNode));
			}
		}
		LogOutput(LOG_TAG_RENDERER, "OK\n");
		return true;
	}
	catch (int err) {
		Destroy();
		LogOutput(LOG_TAG_RENDERER, "Fail(%d)\n", err);
		return false;
	}
}

bool CGLES3Material::LoadPass(TiXmlNode *pPassNode)
{
	uint32_t name = HashValue(pPassNode->ToElement()->AttributeString("name"));

	if (m_pPasses[name] == nullptr) {
		m_pPasses[name] = new CGLES3MaterialPass(name);
		return m_pPasses[name]->Load(pPassNode);
	}

	return true;
}

void CGLES3Material::Destroy(void)
{
	for (const auto &itPass : m_pPasses) {
		delete itPass.second;
	}

	m_pPasses.clear();
}

CGfxMaterialPass* CGLES3Material::GetPass(uint32_t name) const
{
	const auto &itPass = m_pPasses.find(name);
	return itPass != m_pPasses.end() ? itPass->second : nullptr;
}
