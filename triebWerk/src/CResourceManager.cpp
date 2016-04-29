#include <CResourceManager.h>

triebWerk::CResourceManager::CResourceManager() :
	m_pGraphicsHandle(nullptr)
{
}

triebWerk::CResourceManager::~CResourceManager()
{
}

bool triebWerk::CResourceManager::Initialize(CGraphics* a_pGraphics)
{
	bool error = SetModulPath();
	m_pGraphicsHandle = a_pGraphics;
	return error;
}

void triebWerk::CResourceManager::CleanUp()
{
	for (auto Tileset : m_TilesetBuffer)
	{
		Tileset.second->ClearMap();
		delete Tileset.second;
	}

	for (auto config : m_ConfigurationBuffer)
	{
		delete config.second;
	}

	for (auto mesh : m_MeshBuffer)
	{
		mesh.second->m_pVertexBuffer->Release();
		delete mesh.second->m_pVertices;
		delete mesh.second;
	}

	for (auto texture : m_TextureBuffer)
	{
		texture.second->GetD3D11Texture()->Release();
		texture.second->GetShaderResourceView()->Release();
		delete texture.second;
	}

	for (auto material : m_MaterialBuffer)
	{
		delete material.second;
	}

	m_ConfigurationBuffer.clear();
	m_TextureBuffer.clear();
	m_TilesetBuffer.clear();
	m_MeshBuffer.clear();
}

const char& triebWerk::CResourceManager::GetModulPath()
{
	return *m_ModulPath.c_str();
}

void triebWerk::CResourceManager::LoadAllFilesInFolder(const char * a_pPath)
{
	std::vector<SFile> filesToLoad = SearchFolderForAllFiles(a_pPath);

	for(auto file : filesToLoad)
	{
		LoadFile(file);
	}
}

void triebWerk::CResourceManager::LoadAllSpecificFilesInFolder(EFileType a_FileType, const char * a_pPath)
{
	std::vector<SFile> filesToLoad = SearchFolderForAllFiles(a_pPath);

	for (auto file : filesToLoad)
	{
		if (file.FileType == a_FileType)
		{
			LoadFile(file);
		}
	}
}

void triebWerk::CResourceManager::LoadSpecificFile(const char * a_pPath)
{
	SFile fileToLoad;
	fileToLoad.FileType = GetFileType(a_pPath);
	fileToLoad.FilePath = m_ModulPath + a_pPath;
	fileToLoad.FileName = AbstractFileNameFromPath(a_pPath);
}

triebWerk::CTilesetMap * triebWerk::CResourceManager::GetTileset(const char * TilesetName)
{
	auto foundIterator = m_TilesetBuffer.find(StringHasher(TilesetName));

	if (foundIterator == m_TilesetBuffer.end())
	{
		return nullptr;
	}
	else
	{
		return foundIterator->second;
	}
}

triebWerk::CConfiguration* triebWerk::CResourceManager::GetConfiguration(const char * a_pConfiguration)
{
	auto foundIterator = m_ConfigurationBuffer.find(StringHasher(a_pConfiguration));

	if (foundIterator == m_ConfigurationBuffer.end())
	{
		return nullptr;
	}
	else
	{
		return foundIterator->second;
	}
}

triebWerk::CTexture2D * triebWerk::CResourceManager::GetTexture2D(const char * a_pTexture2DName)
{
	auto foundIterator = m_TextureBuffer.find(StringHasher(a_pTexture2DName));

	if (foundIterator == m_TextureBuffer.end())
	{
		return nullptr;
	}
	else
	{
		return foundIterator->second;
	}
}

triebWerk::CMesh * triebWerk::CResourceManager::GetMesh(const char * a_pMeshName)
{
	auto foundIterator = m_MeshBuffer.find(StringHasher(a_pMeshName));

	if (foundIterator == m_MeshBuffer.end())
	{
		return nullptr;
	}
	else
	{
		return foundIterator->second;
	}
}

triebWerk::CMaterial * triebWerk::CResourceManager::GetMaterial(const char * a_pEffectName)
{
	auto foundIterator = m_MaterialBuffer.find(StringHasher(a_pEffectName));

	if (foundIterator == m_MaterialBuffer.end())
	{
		return nullptr;
	}
	else
	{
		return foundIterator->second;
	}
}

void triebWerk::CResourceManager::UnloadTileset(const char * a_TilesetName)
{
	auto foundIterator = m_TilesetBuffer.find(StringHasher(a_TilesetName));

	if (foundIterator == m_TilesetBuffer.end())
	{
		return;
	}
	else
	{
		foundIterator->second->ClearMap();
		delete foundIterator->second;
		m_TilesetBuffer.erase(StringHasher(a_TilesetName));
	}
}

void triebWerk::CResourceManager::UnloadConfiguration(const char * a_pConfigurationName)
{
	auto foundIterator = m_ConfigurationBuffer.find(StringHasher(a_pConfigurationName));

	if (foundIterator == m_ConfigurationBuffer.end())
	{
		return;
	}
	else
	{
		delete foundIterator->second;
		m_ConfigurationBuffer.erase(StringHasher(a_pConfigurationName));
	}
}

void triebWerk::CResourceManager::UnloadTexture2D(const char * a_pTexture2DName)
{
	auto foundIterator = m_TextureBuffer.find(StringHasher(a_pTexture2DName));

	if (foundIterator == m_TextureBuffer.end())
	{
		return;
	}
	else
	{
		foundIterator->second->GetD3D11Texture()->Release();
		foundIterator->second->GetShaderResourceView()->Release();
		delete foundIterator->second;

		m_TextureBuffer.erase(StringHasher(a_pTexture2DName));
	}
}

void triebWerk::CResourceManager::UnloadMesh(const char * a_pMeshName)
{
	auto foundIterator = m_MeshBuffer.find(StringHasher(a_pMeshName));

	if (foundIterator == m_MeshBuffer.end())
	{
		return;
	}
	else
	{
		foundIterator->second->m_pVertexBuffer->Release();
		delete foundIterator->second->m_pVertices;
		delete foundIterator->second;
		m_MeshBuffer.erase(StringHasher(a_pMeshName));
	}
}

void triebWerk::CResourceManager::LoadFile(SFile a_File)
{
	switch (a_File.FileType)
	{
	case EFileType::HLSL:
		LoadHLSL(a_File);
		break;
	case EFileType::INI:
		LoadINI(a_File);
		break;
	case EFileType::MP3:
		LoadMP3(a_File);
		break;
	case EFileType::OBJ:
		LoadOBJ(a_File);
		break;
	case EFileType::PNG:
		LoadPNG(a_File);
		break;
	case EFileType::TMX:
		LoadTMX(a_File);
		break;
	}
}

void triebWerk::CResourceManager::LoadPNG(SFile a_File)
{
	CTexture2D* texture = new CTexture2D();

	unsigned int width;
	unsigned int height;

	unsigned int error = lodepng::decode(texture->m_PixelData, width, height, a_File.FilePath);

	if (error != 0)
		return;
	else
	{
		ID3D11Texture2D* d3dtexture = m_pGraphicsHandle->CreateD3D11Texture2D(&texture->m_PixelData[0], width, height);

		ID3D11ShaderResourceView* resourceView = m_pGraphicsHandle->CreateID3D11ShaderResourceView(d3dtexture);

		texture->SetTexture(width, height, d3dtexture, resourceView);

		m_TextureBuffer.insert(CTexturePair(StringHasher(a_File.FileName), texture));
	}

}

void triebWerk::CResourceManager::LoadOBJ(SFile a_File)
{
	COBJParser objParser;
	objParser.LoadOBJ(a_File.FilePath.c_str());

	CMesh* mesh = new CMesh();
	mesh->m_VertexCount = objParser.m_VertexCount;
	mesh->m_pVertices = objParser.m_pVertices;

	mesh->m_pVertexBuffer = m_pGraphicsHandle->CreateVertexBuffer(mesh->m_pVertices, mesh->m_VertexCount);

	m_MeshBuffer.insert(CMeshPair(StringHasher(a_File.FileName), mesh));
}

void triebWerk::CResourceManager::LoadMP3(SFile a_File)
{
}

void triebWerk::CResourceManager::LoadTMX(SFile a_File)
{
	CTMXParser parserTMX = CTMXParser();
	CTilesetMap* tileset = parserTMX.ParseData(a_File.FilePath.c_str());

	for (size_t i = 0; i < tileset->m_Layers.size(); i++)
	{
		if (tileset->m_Layers[i]->GetType() == IMapLayer::ETypes::Type::MapImageLayer)
		{
			CMapImageLayer* t = (CMapImageLayer*)tileset->m_Layers[i];
			//std::string imageName = AbstractFileNameFromPath(t->m_ImageName).c_str();
			//LoadTexture(AbstractFolderFromPath(t->m_ImageName).c_str(), imageName.c_str());
			//t->m_ImageHeight = GetTextureByName(imageName.c_str())->GetHeight();
			//t->m_ImageWidth = GetTextureByName(imageName.c_str())->GetWidth();
		}

		if (tileset->m_Layers[i]->GetType() == IMapLayer::ETypes::Type::MapLayer)
		{
			CMapLayer* t = (CMapLayer*)tileset->m_Layers[i];
			//t->m_TileSetTexture = GetTextureByName(t->m_TilesetName.c_str());
		}

		if (tileset->m_Layers[i]->GetType() == IMapLayer::ETypes::Type::ObjectLayer)
		{
			CObjectLayer* t = (CObjectLayer*)tileset->m_Layers[i];
			//t->m_TileSetTexture = GetTextureByName(t->m_TilesetName.c_str());
			int a = 0;
		}
	}

	m_TilesetBuffer.insert(CTilesetPair(StringHasher(RemoveFileType(a_File.FileName)), tileset));
}

void triebWerk::CResourceManager::LoadHLSL(SFile a_File)
{
	CHLSLParser hlslParser;
	CMaterial* pMaterial = hlslParser.ParseShader(a_File.FilePath.c_str(), m_pGraphicsHandle);

	m_MaterialBuffer.insert(CMaterialPair(StringHasher(RemoveFileType(a_File.FileName)), pMaterial));
}

void triebWerk::CResourceManager::LoadINI(SFile a_File)
{
	CINIParser iniParser;
	CConfiguration* configuration = iniParser.ParseData(a_File.FilePath.c_str());
	
	if (configuration == nullptr)
		return;

	m_ConfigurationBuffer.insert(CConfigurationPair(StringHasher(a_File.FileName), configuration));
}

bool triebWerk::CResourceManager::SetModulPath()
{
	char modulFilePath[MAX_PATH] = "";
	if (GetModuleFileNameA(NULL, modulFilePath, MAX_PATH) == 0)
		return false;

	m_ModulPath = modulFilePath;
	m_ModulPath.resize(m_ModulPath.rfind("\\") + 1);

	return true;
}

std::vector<triebWerk::CResourceManager::SFile> triebWerk::CResourceManager::SearchFolderForAllFiles(const char * a_FolderToLoad)
{
	WIN32_FIND_DATA winFindData;
	HANDLE hFindHandle = INVALID_HANDLE_VALUE;

	//Build Directory String
	std::string dir = m_ModulPath;
	dir += a_FolderToLoad;
	dir += "\\";

	//Build Search Directory String
	std::string SearchFolderForAllFiles = dir;
	SearchFolderForAllFiles += "*";

	std::vector<SFile>filesToLoad; 
	filesToLoad.clear();
	//Find first Element in Directory
	hFindHandle = FindFirstFile(SearchFolderForAllFiles.c_str(), &winFindData);

	std::vector<std::string>directories;

	if (INVALID_HANDLE_VALUE == hFindHandle)
	{
		//Debug.m_Logfile.LogfText(Debug.m_Logfile.Red, false, "Folder not found! Name: %s", a_FolderToLoad);
		return filesToLoad;
	}

	//Search for Files and Directories
	do
	{
		if (winFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (strcmp(winFindData.cFileName, ".") != 0 && strcmp(winFindData.cFileName, "..") != 0)
			{
				std::string directoryPath = a_FolderToLoad;
				directoryPath += "\\";
				directoryPath += winFindData.cFileName;
				directories.push_back(directoryPath);
			}
		}
		else
		{
			SFile file;
			file.FileName = winFindData.cFileName;
			file.FilePath = dir + winFindData.cFileName;
			file.FileType = GetFileType(file.FileName);

			filesToLoad.push_back(file);
		}
	} while (FindNextFileA(hFindHandle, &winFindData) != 0);

	//Serach All Files in Subdirectories
	for (auto directory : directories)
	{
		std::vector<SFile> filesInDirectory = this->SearchFolderForAllFiles(directory.c_str());

		for (auto file : filesInDirectory)
		{
			filesToLoad.push_back(file);
		}
	}

	return filesToLoad;
}

bool triebWerk::CResourceManager::CompareFileTypes(std::string a_Name, const char * a_ExpectedType)
{
	int a = a_Name.compare(a_Name.find("."), std::string::npos, a_ExpectedType);
	if (a == 0)
		return true;

	return false;
}

std::string triebWerk::CResourceManager::RemoveFileType(std::string a_Name)
{
	return a_Name.substr(0, a_Name.find("."));
}

std::string triebWerk::CResourceManager::AbstractFileNameFromPath(std::string a_Path)
{
	return a_Path.substr(a_Path.rfind("\\") + 1, a_Path.size() - a_Path.rfind("\\") + 1);
}

std::string triebWerk::CResourceManager::AbstractFolderFromPath(std::string a_Path)
{
	size_t end = a_Path.rfind("/", a_Path.rfind("/"));
	size_t start = a_Path.rfind("/", a_Path.rfind("/") - 1) + 1;
	return a_Path.substr(start, end - start);
}

triebWerk::EFileType triebWerk::CResourceManager::GetFileType(std::string a_FileName)
{
	std::string fileType = a_FileName.substr(a_FileName.find("."), a_FileName.size() - a_FileName.find("."));

	if (fileType == ".ini")
	{
		return EFileType::INI;
	}
	else if(fileType == ".hlsl")
	{
		return EFileType::HLSL;
	}
	else if (fileType == ".mp3")
	{
		return EFileType::MP3;
	}
	else if (fileType == ".obj")
	{
		return EFileType::OBJ;
	}
	else if (fileType == ".png")
	{
		return EFileType::PNG;
	}
	else if (fileType == ".tmx")
	{
		return EFileType::TMX;
	}

	return EFileType::NONE;
}
