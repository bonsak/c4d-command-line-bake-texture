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
    
    // check if there is something to do
    if(textureTagArray.GetCount() == 0)
        return true;
    
    // setup bake settings
    Int32 BakeWidth = 256;
    Int32 BakeHeight = 256;
    BaseContainer settings;
    settings.SetInt32(BAKE_TEX_WIDTH,BakeWidth);
    settings.SetInt32(BAKE_TEX_HEIGHT, BakeHeight);
    settings.SetInt32(BAKE_TEX_PIXELBORDER, 0);
    settings.SetBool(BAKE_TEX_CONTINUE_UV, true);
    settings.SetBool(BAKE_TEX_USE_PHONG_TAG, true);
    settings.SetVector(BAKE_TEX_FILL_COLOR, Vector(0.0));
    settings.SetBool(BAKE_TEX_COLOR, true);
    settings.SetBool(BAKE_TEX_COLOR_ILLUM, false);
    settings.SetBool(BAKE_TEX_COLOR_SHADOWS, true);
    settings.SetBool(BAKE_TEX_COLOR_LUMINANCE, false);
    settings.SetBool(BAKE_TEX_COLOR_DIFFUSION, false);
//    settings.SetBool(BAKE_TEX_LUMINANCE, true);
    
    void SetName( const String & name );

    // InitBakeTexure
    BAKE_TEX_ERR err;
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
    // if success, do bake
    if(err == BAKE_TEX_ERR_NONE)
    {
        const String LayerNameOne = "Shadow";
        const String LayerNameTwo = "Gi";
        // prepare multipass bitmap
        MultipassBitmap* bitmap = MultipassBitmap::Alloc(BakeWidth,BakeHeight,COLORMODE_RGB);
        // bake
        BakeTexture(bakeDoc,settings,bitmap,nullptr,nullptr,nullptr);
        // show result
//        Int NumLayers = bitmap->GetLayerCount();
        
        MultipassBitmap* layerOne = bitmap->AddLayer( bitmap, COLORMODE_RGB );
//        layerOne->SetName(LayerNameOne);
        MultipassBitmap* layerTwo = bitmap->AddLayer( layerOne, COLORMODE_RGB );
//        layerTwo->SetName(LayerNameTwo);
        
        ShowBitmap(bitmap);
        // clear result
        MultipassBitmap::Free(bitmap);
    }
    else if(err != BAKE_TEX_ERR_NONE)
    {
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
