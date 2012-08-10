xof 0303txt 0032
template Frame {
 <3d82ab46-62da-11cf-ab39-0020af71e433>
 [...]
}

template Matrix4x4 {
 <f6f23f45-7686-11cf-8f52-0040333594a3>
 array FLOAT matrix[16];
}

template FrameTransformMatrix {
 <f6f23f41-7686-11cf-8f52-0040333594a3>
 Matrix4x4 frameMatrix;
}

template Vector {
 <3d82ab5e-62da-11cf-ab39-0020af71e433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template MeshFace {
 <3d82ab5f-62da-11cf-ab39-0020af71e433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template Mesh {
 <3d82ab44-62da-11cf-ab39-0020af71e433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

template MeshNormals {
 <f6f23f43-7686-11cf-8f52-0040333594a3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template Coords2d {
 <f6f23f44-7686-11cf-8f52-0040333594a3>
 FLOAT u;
 FLOAT v;
}

template MeshTextureCoords {
 <f6f23f40-7686-11cf-8f52-0040333594a3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template ColorRGBA {
 <35ff44e0-6c7c-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template IndexedColor {
 <1630b820-7842-11cf-8f52-0040333594a3>
 DWORD index;
 ColorRGBA indexColor;
}

template MeshVertexColors {
 <1630b821-7842-11cf-8f52-0040333594a3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template ColorRGB {
 <d3e16e81-7835-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template Material {
 <3d82ab4d-62da-11cf-ab39-0020af71e433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshMaterialList {
 <f6f23f42-7686-11cf-8f52-0040333594a3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material <3d82ab4d-62da-11cf-ab39-0020af71e433>]
}

template TextureFilename {
 <a42790e1-7810-11cf-8f52-0040333594a3>
 STRING filename;
}

template VertexElement {
 <f752461c-1e23-48f6-b9f8-8350850f336f>
 DWORD Type;
 DWORD Method;
 DWORD Usage;
 DWORD UsageIndex;
}

template DeclData {
 <bf22e553-292c-4781-9fea-62bd554bdd93>
 DWORD nElements;
 array VertexElement Elements[nElements];
 DWORD nDWords;
 array DWORD data[nDWords];
}


Frame DXCC_ROOT {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Frame persp {
  

  FrameTransformMatrix {
   0.999507,-0.000000,0.031411,0.000000,0.031407,0.016377,-0.999373,0.000000,-0.000514,0.999866,0.016369,0.000000,0.003024,-5.878794,-0.096240,1.000000;;
  }

  Frame perspShape {
   

   FrameTransformMatrix {
    1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
   }
  }
 }

 Frame top {
  

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,-1.000000,0.000000,0.000000,0.000000,100.099998,0.000000,1.000000;;
  }

  Frame topShape {
   

   FrameTransformMatrix {
    1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
   }
  }
 }

 Frame front {
  

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,-100.099998,1.000000;;
  }

  Frame frontShape {
   

   FrameTransformMatrix {
    1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
   }
  }
 }

 Frame side {
  

  FrameTransformMatrix {
   0.000000,0.000000,1.000000,0.000000,0.000000,1.000000,0.000000,0.000000,-1.000000,0.000000,0.000000,0.000000,100.099998,0.000000,0.000000,1.000000;;
  }

  Frame sideShape {
   

   FrameTransformMatrix {
    1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
   }
  }
 }

 Frame pPlane1 {
  

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
  }

  Frame pPlaneShape1 {
   

   FrameTransformMatrix {
    1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
   }

   Mesh pPlaneShape1_Mesh {
    4;
    -1.000000;-0.000000;-1.000000;,
    -1.000000;0.000000;1.000000;,
    1.000000;-0.000000;-1.000000;,
    1.000000;0.000000;1.000000;;
    2;
    3;0,1,2;,
    3;2,1,3;;

    MeshNormals {
     4;
     0.000000;1.000000;-0.000000;,
     0.000000;1.000000;-0.000000;,
     0.000000;1.000000;-0.000000;,
     0.000000;1.000000;-0.000000;;
     2;
     3;0,1,2;,
     3;2,1,3;;
    }

    MeshTextureCoords {
     4;
     0.000000;1.000000;,
     0.000000;0.000000;,
     1.000000;1.000000;,
     1.000000;0.000000;;
    }

    MeshVertexColors {
     4;
     0;0.000000;0.000000;0.000000;0.000000;;,
     1;0.000000;0.000000;0.000000;0.000000;;,
     2;0.000000;0.000000;0.000000;0.000000;;,
     3;0.000000;0.000000;0.000000;0.000000;;;
    }

    MeshMaterialList {
     1;
     2;
     0,
     0;

     Material {
      0.800000;0.800000;0.800000;1.000000;;
      0.000000;
      1.000000;1.000000;1.000000;;
      0.000000;0.000000;0.000000;;

      TextureFilename {
       "Z:/Temporal Shift/Cypher Edge Interactive/Code/Main Build/Bin/Textures/target.tga";
      }
     }
    }

    DeclData {
     2;
     2;0;6;0;,
     2;0;7;0;;
     24;
     1065353216,
     414956846,
     851164462,
     2998648110,
     2776629247,
     3212836863,
     1065353216,
     414956846,
     851164462,
     2998648110,
     2776629247,
     3212836863,
     1065353216,
     414956846,
     851164462,
     2998648110,
     2776629247,
     3212836863,
     1065353216,
     414956846,
     851164462,
     2998648110,
     2776629247,
     3212836863;
    }
   }
  }
 }
}