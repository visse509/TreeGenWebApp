#include "main.h"
#include "tree.h"
#include <fbxsdk.h>
#include <iostream>
#include <fstream>

void exportToFBX(Tree *tree,std::string resPath);
void exportToModel(Tree *tree,std::string resPath);
std::string printBranch(Tree *tree);
FbxNode* addTreetoScene(FbxScene* pScene, const char* pName,Tree *tree);
FbxNode* addBranchToScene(FbxScene* pScene, const char* pName,Tree *tree);
void CreateMaterials(FbxScene* pScene, FbxMesh* pMesh,std::vector<unsigned short> indexList);
FbxVector4 convert(Vert * v);
static int nPolygonsAdded = 0;
static const char* gDiffuseElementName = "DiffuseUV";
static const char* gAmbientElementName = "AmbientUV";
static const char* gEmissiveElementName = "EmissiveUV";

int main(int argc, char **argv)
{
	remove("go");
	std::string propXml;
	std::string lsXml;
	std::string resPath;
	if(argc >= 2){
		propXml = argv[1];
	}
	if (argc >= 3){
		lsXml = argv[2];
	}
	if (argc >= 4){
		resPath = argv[3];
	}
	if(propXml.empty()){
	//	propXml = "treeProp.xml";
	}
	if(lsXml.empty()){
		//lsXml = "treeString.xml";
	}
	if(resPath.empty()){
		lsXml = "res.fbx";
	}
	Tree *tree = new Tree(-1,0,propXml,lsXml);
	tree->createTreeLS();
	
	exportToFBX(tree,resPath);
	exportToModel(tree,resPath);
}

void exportToModel(Tree *tree,std::string resPath){
	std::ofstream myfile;
	std::string filePath = resPath;
	filePath.append(".xml");
	myfile.open(filePath);
	myfile << "<root>\n";
	myfile << "\t<trunk>\n";
	myfile << printBranch(tree);
	myfile << "\t</trunk>\n";
	myfile << "</root>\n";
	myfile.close();
}

std::string printBranch(Tree *tree){
	std::string res;
	res.append("\t\t<data>\n");
	for(int i = 0; i < tree->trunk->getMeshData()->vertexList.size(); i++){
		std::string data = Vert::VertToString(tree->trunk->getMeshData()->vertexList[i]);
		data.append("\n");
		res.append( data);
	}
	res.append( "\t\t</data>\n");

	res.append( "\t\t<indices>\n");
	for(int i = 0; i < tree->trunk->getMeshData()->index.size(); i++){

		std::ostringstream convert;
		std::string data = static_cast<std::ostringstream*>( &(std::ostringstream() << tree->trunk->getMeshData()->index[i]) )->str();
		data.append("\n");
		res.append( data);
	}
	res.append( "\t\t</indices>\n");


	res.append( "\t\t<branches>\n");
	
	for(int i = 0; i < tree->branches.size(); i++){
		res.append( "\t\t\t<branch>\n");
		res.append(printBranch(tree->branches[i]));
		res.append( "\t\t\t</branch>\n");
	}
	
	res.append( "\t\t</branches>\n");

	return res;
}


void exportToFBX(Tree *tree,std::string resPath){
	FbxManager * sdkManager = NULL;
	sdkManager = FbxManager::Create();
	FbxScene* myScene = FbxScene::Create(sdkManager, "");

	FbxNode* treeMesh = addBranchToScene(myScene, "Cube",tree);
	FbxNode* lRootNode = myScene->GetRootNode();
	
	lRootNode->AddChild(treeMesh);;

	

	FbxIOSettings * ios = FbxIOSettings::Create(sdkManager, IOSROOT );
	sdkManager->SetIOSettings(ios);
	FbxExporter* exporter = FbxExporter::Create(sdkManager, "");
	std::string filePath = resPath;
	filePath.append(".fbx");
	bool status = exporter->Initialize(filePath.c_str(), -1, sdkManager->GetIOSettings());
	exporter->Export(myScene);
	std::ofstream myfile;
	myfile.open("go");
	myfile.close();
}
FbxVector4 convert(Vert * v){
	FbxVector4 res;
	res.Set(v->x,v->y,v->z);
	return res;
}

std::vector<Vert*> getVertices(Tree *tree){
	static std::vector<Vert*> vresList;
	vresList.insert( vresList.end(), tree->trunk->getMeshData()->vertexList.begin(), tree->trunk->getMeshData()->vertexList.end() );
	for(int i  = 0; i < tree->branches.size();i++){
		getVertices(tree->branches[i]);
	}
	return vresList;
}

std::vector<unsigned short> getIndices(Tree *tree){
	static std::vector<unsigned short> iresList;
	if(iresList.size() == 0){
		iresList = tree->trunk->getMeshData()->index;
	}else{
		int size = iresList.size()-1;
		int lastValue = iresList[size];
		for(int i = 0; i < tree->trunk->getMeshData()->index.size(); i++){
			iresList.push_back( tree->trunk->getMeshData()->index[i]+lastValue+1);
		}
	}
	for(int i  = 0; i < tree->branches.size();i++){
		getIndices(tree->branches[i]);
	}
	return iresList;
}

FbxNode* addBranchToScene(FbxScene* pScene, const char* pName,Tree *tree)
{
	FbxMesh* lMesh = FbxMesh::Create(pScene, pName);

	std::vector<Vert*> vertList;
	vertList = getVertices(tree);

	std::vector<unsigned short> indexList;
	indexList = getIndices(tree);

	int nPolygons = indexList.size()/3;
    int i, j;
	std::vector<FbxVector4> normals;
    lMesh->InitControlPoints(vertList.size());
    FbxVector4* lControlPoints = lMesh->GetControlPoints();

	for(int i = 0; i < vertList.size(); i++){
		Vert *v = vertList[i];
   		lControlPoints[i] = convert(v);
		
	}
	for(int i = 0; i < vertList.size()-2; i = i+3){
		Vert *v1 = vertList[i];
		Vert *v2 = vertList[i+1];
		Vert *v3 = vertList[i+2];
		normals.push_back(convert(Vert::getNormalVector(v1,v2,v3)));
		
	}
    // specify normals per control point.

    FbxGeometryElementNormal* lNormalElement= lMesh->CreateElementNormal();
    lNormalElement->SetMappingMode(FbxGeometryElement::eByControlPoint);
    lNormalElement->SetReferenceMode(FbxGeometryElement::eDirect);

	for(int i = 0; i < (vertList.size()/3)-2; i++)
	{
		lNormalElement->GetDirectArray().Add(normals[i]);
		lNormalElement->GetDirectArray().Add(normals[i]);
		lNormalElement->GetDirectArray().Add(normals[i]);
	}

    // Set material mapping.
    FbxGeometryElementMaterial* lMaterialElement = lMesh->CreateElementMaterial();
    lMaterialElement->SetMappingMode(FbxGeometryElement::eByPolygon);
    lMaterialElement->SetReferenceMode(FbxGeometryElement::eIndexToDirect);

	for(int j = 0; j < nPolygons; j++){
		lMesh->BeginPolygon(j);
		for(int i = j*3;i < (j*3)+3;i++){
			lMesh->AddPolygon(indexList[i]);
		}
		lMesh->EndPolygon();
	}

    FbxNode* lNode = FbxNode::Create(pScene,pName);
    lNode->SetNodeAttribute(lMesh);
    CreateMaterials(pScene, lMesh,indexList);
    return lNode;
}

void CreateMaterials(FbxScene* pScene, FbxMesh* pMesh,std::vector<unsigned short> indexList)
{
  //  int i;

	//for (i = 0; i < (indexList.size()/3)+2; i++ )
   // {
        FbxString lMaterialName = "material";
        FbxString lShadingName = "Phong";
        lMaterialName = 0;
        FbxDouble3 lBlack(0.0, 0.0, 0.0);
        FbxDouble3 lRed(1.0, 0.0, 0.0);
        FbxDouble3 lColor;
        FbxSurfacePhong *lMaterial = FbxSurfacePhong::Create(pScene, lMaterialName.Buffer());


        // Generate primary and secondary colors.
        lMaterial->Emissive.Set(lBlack);
        lMaterial->Ambient.Set(lRed);
        lColor = FbxDouble3(0 > 2   ? 1.0 : 0.0, 
            0 > 0 && 0 < 4 ? 1.0 : 0.0, 
            0 % 2   ? 0.0 : 1.0);
        lMaterial->Diffuse.Set(lColor);
        lMaterial->TransparencyFactor.Set(0.0);
        lMaterial->ShadingModel.Set(lShadingName);
        lMaterial->Shininess.Set(0.5);

        //get the node of mesh, add material for it.
        FbxNode* lNode = pMesh->GetNode();
        if(lNode)             
            lNode->AddMaterial(lMaterial);
   // }  
}
main::~main(void)
{
}
