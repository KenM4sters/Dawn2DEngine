#pragma once
#include "Camera.hpp"
#include <iostream>
#include <vector>

class AssetManager 
{
    public:

        AssetManager(const AssetManager& other) = delete;

        static void Delete() { delete mCamera; }

        static void SubmitCamera(OrthographicCamera* camera) { mCamera = camera; }

        static OrthographicCamera* mCamera;
};