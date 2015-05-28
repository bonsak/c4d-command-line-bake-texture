// example code for a menu plugin and multiprocessing

#include "c4d.h"
#include "c4d_symbols.h"
#include "c4d_tools.h"
#include "bake_texture.h"
#include "c4d_general.h"
#include "c4d_memory_mp.h"
#include "main.h"


class MenuTest : public CommandData
{
public:
	virtual Bool Execute(BaseDocument* doc);
};

Bool MenuTest::Execute(BaseDocument *doc)
{
    BaseObject *obj = doc->GetFirstObject();
    // let's use the same UVW tag for all texture tags
    UVWTag* uvwTag = (UVWTag*)obj->GetTag(Tuvw);
    
    GePrint("We got to if(uvwTag");
    if(uvwTag == nullptr)
        return true;
    
    BaseTag* tag = obj->GetFirstTag();
    
    // prepare arrays for texture tag ptr and uvw tag ptr
    maxon::BaseArray<TextureTag*> textureTagArray;
    maxon::BaseArray<UVWTag*> uvwTags;
    
    // save ptr of all texture tags; fill uvw array
    while(tag)
    {
        GePrint(tag->GetName());
        if(tag->GetType() == Ttexture)
        {
            textureTagArray.Append((TextureTag*)tag);
            uvwTags.Append((UVWTag*)uvwTag);
        }
        
        tag = tag->GetNext();
    }
    
    GePrint("We got to textureTagArray");
    // check if there is something to do
    if(textureTagArray.GetCount() == 0)
        return true;
    
    // setup bake settings
    BaseContainer settings;
    settings.SetInt32(BAKE_TEX_WIDTH,1024);
    settings.SetInt32(BAKE_TEX_HEIGHT, 1024);
    settings.SetInt32(BAKE_TEX_PIXELBORDER, 0);
    settings.SetBool(BAKE_TEX_CONTINUE_UV, true);
    settings.SetBool(BAKE_TEX_USE_PHONG_TAG, true);
    settings.SetVector(BAKE_TEX_FILL_COLOR, Vector(0.0));
    settings.SetBool(BAKE_TEX_COLOR, true);
    settings.SetBool(BAKE_TEX_COLOR_ILLUM, false);
    settings.SetBool(BAKE_TEX_COLOR_SHADOWS, false);
    settings.SetBool(BAKE_TEX_COLOR_LUMINANCE, false);
    settings.SetBool(BAKE_TEX_COLOR_DIFFUSION, false);
    
    // InitBakeTexure
    BAKE_TEX_ERR err;
    GePrint("We got to InitBakeTexture");
    BaseDocument* bakeDoc = InitBakeTexture(
                                            doc,
                                            textureTagArray.GetFirst(),
                                            uvwTags.GetFirst(),
                                            nullptr,
                                            textureTagArray.GetCount(),
                                            settings,
                                            &err,
                                            nullptr
                                            );
    GePrint("We passed InitBakeTexture");
    // if success, do bake
    if(err == BAKE_TEX_ERR_NONE)
    {
        GePrint("We're baking!");
        // prepare multipass bitmap
        MultipassBitmap* bitmap = MultipassBitmap::Alloc(1024,1024,COLORMODE_RGB);
        // bake
        BakeTexture(bakeDoc,settings,bitmap,nullptr,nullptr,nullptr);
        // show result
        ShowBitmap(bitmap);
        // clear result
        MultipassBitmap::Free(bitmap);
    }
    else if(err != BAKE_TEX_ERR_NONE)
    {
        GePrint("We're not baking!");
        MessageDialog(GeLoadString(err));
    }
    
    // clear bake document
    BaseDocument::Free(bakeDoc);
}

Bool RegisterMenuTest(void)
{
	// be sure to use a unique ID obtained from www.plugincafe.com
	return RegisterCommandPlugin(1000956, GeLoadString(IDS_MENUTEST), 0, AutoBitmap("icon.tif"), String("C++ SDK Menu Test Plugin"), NewObjClear(MenuTest));
}
