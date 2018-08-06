// 3dsmodel.cpp
//
// Copyright (C) 2000, Chris Laurel <claurel@shatters.net>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#include "3dsmodel.h"

#include <utility>

using namespace Eigen;
using namespace std;


M3DColor::M3DColor() :
    red(0), green(0), blue(0)
{
}

M3DColor::M3DColor(float _red, float _green, float _blue) :
    red(_red), green(_green), blue(_blue)
{
}

string M3DMaterial::getName() const
{
    return name;
}

void M3DMaterial::setName(string _name)
{
    name = std::move(_name);
}

M3DColor M3DMaterial::getDiffuseColor() const
{
    return diffuse;
}

void M3DMaterial::setDiffuseColor(M3DColor color)
{
    diffuse = color;
}

M3DColor M3DMaterial::getAmbientColor() const
{
    return ambient;
}

void M3DMaterial::setAmbientColor(M3DColor color)
{
    ambient = color;
}

M3DColor M3DMaterial::getSpecularColor() const
{
    return specular;
}

void M3DMaterial::setSpecularColor(M3DColor color)
{
    specular = color;
}

float M3DMaterial::getShininess() const
{
    return shininess;
}

void M3DMaterial::setShininess(float _shininess)
{
    shininess = _shininess;
}

float M3DMaterial::getOpacity() const
{
    return opacity;
}

void M3DMaterial::setOpacity(float _opacity)
{
    opacity = _opacity;
}

string M3DMaterial::getTextureMap() const
{
    return texmap;
}

void M3DMaterial::setTextureMap(const string& _texmap)
{
    texmap = _texmap;
}


M3DTriangleMesh::M3DTriangleMesh()
{
    matrix = Matrix4f::Identity();
}

Matrix4f M3DTriangleMesh::getMatrix() const
{
    return matrix;
}

void M3DTriangleMesh::setMatrix(const Matrix4f& m)
{
    matrix = m;
}

Vector3f M3DTriangleMesh::getVertex(uint16_t n) const
{
    return points[n];
}

uint16_t M3DTriangleMesh::getVertexCount() const
{
    return (uint16_t) (points.size());
}

void M3DTriangleMesh::addVertex(const Vector3f& p)
{
    points.push_back(p);
}

Vector2f M3DTriangleMesh::getTexCoord(uint16_t n) const
{
    return texCoords[n];
}

uint16_t M3DTriangleMesh::getTexCoordCount() const
{
    return (uint16_t) (texCoords.size());
}

void M3DTriangleMesh::addTexCoord(const Vector2f& p)
{
    texCoords.push_back(p);
}

void M3DTriangleMesh::getFace(uint16_t n, uint16_t& v0, uint16_t& v1, uint16_t& v2) const
{
    int m = (int) n * 3;
    v0 = faces[m];
    v1 = faces[m + 1];
    v2 = faces[m + 2];
}

uint16_t M3DTriangleMesh::getFaceCount() const
{
    return (uint16_t) (faces.size() / 3);
}

void M3DTriangleMesh::addFace(uint16_t v0, uint16_t v1, uint16_t v2)
{
    faces.insert(faces.end(), v0);
    faces.insert(faces.end(), v1);
    faces.insert(faces.end(), v2);
}

uint32_t M3DTriangleMesh::getSmoothingGroups(uint16_t face) const
{
    return face < smoothingGroups.size() ? smoothingGroups[face] : 0;
}

void M3DTriangleMesh::addSmoothingGroups(uint32_t smGroups)
{
    smoothingGroups.insert(smoothingGroups.end(), smGroups);
}

uint16_t M3DTriangleMesh::getSmoothingGroupCount() const
{
    return (uint16_t) (smoothingGroups.size());
}

void M3DTriangleMesh::addMeshMaterialGroup(M3DMeshMaterialGroup* matGroup)
{
    meshMaterialGroups.push_back(matGroup);
}

M3DMeshMaterialGroup* M3DTriangleMesh::getMeshMaterialGroup(uint32_t index) const
{
    return meshMaterialGroups[index];
}

uint32_t M3DTriangleMesh::getMeshMaterialGroupCount() const
{
    return meshMaterialGroups.size();
}


M3DModel::~M3DModel()
{
    for (auto triMesh : triMeshes)
        delete triMesh;
}

M3DTriangleMesh* M3DModel::getTriMesh(uint32_t n)
{
    return n < triMeshes.size() ? triMeshes[n] : nullptr;
}

uint32_t M3DModel::getTriMeshCount()
{
    return triMeshes.size();
}

void M3DModel::addTriMesh(M3DTriangleMesh* triMesh)
{
    triMeshes.insert(triMeshes.end(), triMesh);
}

void M3DModel::setName(const string& _name)
{
    name = _name;
}

const string M3DModel::getName() const
{
    return name;
}


M3DScene::~M3DScene()
{
    for (auto model : models)
        delete model;
    for (auto material : materials)
        delete material;
}

M3DModel* M3DScene::getModel(uint32_t n) const
{
    return n < models.size() ? models[n] : nullptr;
}

uint32_t M3DScene::getModelCount() const
{
    return models.size();
}

void M3DScene::addModel(M3DModel* model)
{
    models.insert(models.end(), model);
}

M3DMaterial* M3DScene::getMaterial(uint32_t n) const
{
    return n < materials.size() ? materials[n] : nullptr;
}

uint32_t M3DScene::getMaterialCount() const
{
    return materials.size();
}

void M3DScene::addMaterial(M3DMaterial* material)
{
    materials.insert(materials.end(), material);
}

M3DColor M3DScene::getBackgroundColor() const
{
    return backgroundColor;
}

void M3DScene::setBackgroundColor(M3DColor color)
{
    backgroundColor = color;
}
