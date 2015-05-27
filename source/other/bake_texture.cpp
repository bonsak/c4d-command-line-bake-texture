//
//  bake_texture.cpp
//  commandline-test
//
//  Created by Bonsak Schieldrop on 27/05/15.
//  Copyright (c) 2015 MAXON Computer GmbH. All rights reserved.
//
#include "c4d.h"
#include "c4d_tools.h"
#include "bake_texture.h"
#include "main.h"

// let's use the same UVW tag for all texture tags
bool BakeTextureTest2(BaseDocument* doc)
{ //Remo: 25.05.2015
    if(nullptr == doc) return false;
    BaseObject *obj = doc->GetActiveObject();
    if (nullptr == obj) return false;
    
    maxon::BaseArray<TextureTag*> textags;
    maxon::BaseArray<UVWTag*> texuvws;
    maxon::BaseArray<UVWTag*> destuvws;
    
    for (BaseTag *tag = obj->GetFirstTag(); tag; tag = tag->GetNext()) {
        if (tag->IsInstanceOf(Ttexture)) {
            TextureTag* texTag = static_cast<TextureTag*>(tag);
            GeConsoleOut(texTag->GetName());
            textags.Append(texTag);
        }
    }
    texuvws.Resize(textags.GetCount());
    destuvws.Resize(textags.GetCount());
    // ! now fill texuvws and destuvws arrays !
    
    BaseContainer settingsBc;
    //fill settingsBc here ... BakeTexEnums
    
    BAKE_TEX_ERR bakeError = BAKE_TEX_ERR_NONE;
    
//    AutoFree<BaseDocument> bakedDoc(
//                                    InitBakeTexture(doc, //The document.
//                                                    &textags[0], //The texture tags to bake. Must be assigned to objects.
//                                                    texuvws.GetFirst(), //The UVW tags to bake.
//                                                    nullptr/*destuvws.GetFirst()*/,  //The destination UVW tags for the bake. If not nullptr, the current projection is transformed into the uvw tags.
//                                                    textags.GetCount(), //The number of tags in textags, texuvws and destuvws arrays.
//                                                    settingsBc, //The bake settings: BakeTexEnums 
//                                                    &bakeError //Assigned the error result, if not nullptr: BAKE_TEX_ERR 
//                                                    )
//                                    );
    
    AutoFree<BaseDocument> bakedSingle(
                                    InitBakeTexture(doc, //The document.
                                                    &textags[0], //The texture tags to bake. Must be assigned to objects.
                                                    texuvws.GetFirst(), //The UVW tags to bake.
                                                    nullptr/*destuvws.GetFirst()*/,  //The destination UVW tags for the bake. If not nullptr, the current projection is transformed into the uvw tags.
                                                    textags.GetCount(), //The number of tags in textags, texuvws and destuvws arrays.
                                                    settingsBc, //The bake settings: BakeTexEnums
                                                    &bakeError //Assigned the error result, if not nullptr: BAKE_TEX_ERR
                                                    )
                                    );
    
    if (BAKE_TEX_ERR_NONE != bakeError) {
        GePrint("bakeError "); // "bakeError ", bakeError
        return false;
    }
    
    //do something with bakedDoc ...
    
    return true;
}