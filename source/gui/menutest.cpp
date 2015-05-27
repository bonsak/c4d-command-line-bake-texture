// example code for a menu plugin and multiprocessing

#include "c4d.h"
#include "c4d_symbols.h"
#include "c4d_tools.h"
#include "bake_texture.h"
//#include "c4d_memory_mp.h"
#include "main.h"

//class MPTest : public C4DThread
//{
//public:
//	MPTest() : _start(0), _result(0.0), _duration(0.0) {}
//	virtual void Main()
//	{
//		Float64 t1 = GeGetMilliSeconds();
//
//		// calculate the 10,000,000 th random number
//		_rnd.Init(_start);
//		for (Int i = 0; i < 10000000; i++)
//			_rnd.Get01();
//
//		_result = _rnd.Get01();
//		_duration = GeGetMilliSeconds() - t1;
//	}
//	virtual const Char* GetThreadName(void) { return "SDK MpTest"; }
//	void SetStart(Int32 start) { _start = start; }
//	Float GetResult() const { return _result; }
//	Float GetDuration() const { return _duration; }
//
//private:
//	Random _rnd;
//	Int32	 _start;
//	Float	 _result;
//	Float	 _duration;
//};
//
//
//class ControlThread : public C4DThread
//{
//public:
//	virtual void Main()
//	{
//		Int32 cnt = GeGetCurrentThreadCount();
//		MPThreadPool mp;
//		maxon::BaseArray<C4DThread*> threadPointers;
//		MPAlloc<MPTest> threads;
//
//		if (!threadPointers.Resize(cnt) || !threads.Init(cnt))
//		{
//			_result = "Initialization failed";
//			return;
//		}
//
//		for (Int32 i = 0; i < cnt; i++)
//		{
//			threads[i].SetStart(i);
//			threadPointers[i] = &threads[i];
//		}
//
//		if (!mp.Init(*this, cnt, threadPointers.GetFirst()))
//		{
//			_result = "MP initialization failed";
//			return;
//		}
//
//		Float64 start_time = GeGetMilliSeconds();
//		if (!mp.Start(THREADPRIORITY_BELOW))
//		{
//			_result = "MP start failed";
//			return;
//		}
//
//		Float64	start_duration = GeGetMilliSeconds() - start_time;
//		mp.Wait();
//		Float64 total_duration = GeGetMilliSeconds() - start_time;
//
//		_result = "Multiprocessing Test on " + String::IntToString(cnt) + " CPUs returns:";
//		for (Int32 i = 0; i < cnt; i++)
//			_result += " " + String::FloatToString(threads[i].GetResult());
//
//		_result += "| Duration per thread:";
//		for (Int32 i = 0; i < cnt; i++)
//		{
//			_result += " " + String::FloatToString(threads[i].GetDuration()) + " ms";
//			if (i + 1 < cnt)
//				_result += ",";
//		}
//
//		_result += "|Thread start: " + String::FloatToString(start_duration) + " ms";
//		_result += "|Total duration: " + String::FloatToString(total_duration) + " ms";
//	}
//	virtual const Char* GetThreadName(void) { return "SDK ControlThread"; }
//	String GetResult() const { return _result; }
//
//private:
//	String _result;
//};
//
class MenuTest : public CommandData
{
public:
	virtual Bool Execute(BaseDocument* doc);
};

Bool MenuTest::Execute(BaseDocument *doc)
{
    BaseObject *obj = doc->GetFirstObject();
    if (!obj) return FALSE;

    BaseContainer bakeSettings;

    bakeSettings.SetBool(BAKE_TEX_USE_CAMERA_VECTOR, FALSE);
    bakeSettings.SetBool(BAKE_TEX_USE_POLYSELECTION, FALSE);
    bakeSettings.SetBool(BAKE_TEX_AMBIENT_OCCLUSION, FALSE);
    bakeSettings.SetBool(BAKE_TEX_NORMAL, FALSE);
    bakeSettings.SetBool(BAKE_TEX_SURFACECOLOR, FALSE);
    bakeSettings.SetBool(BAKE_TEX_COLOR, FALSE);
    bakeSettings.SetBool(BAKE_TEX_DIFFUSION, FALSE);
    bakeSettings.SetBool(BAKE_TEX_LUMINANCE, FALSE);
    bakeSettings.SetBool(BAKE_TEX_ALPHA, FALSE);
    bakeSettings.SetBool(BAKE_TEX_ALPHA, FALSE);
    bakeSettings.SetBool(BAKE_TEX_ILLUMINATION, TRUE);
    bakeSettings.SetBool(BAKE_TEX_SHADOWS, TRUE);

    bakeSettings.SetBool(BAKE_TEX_BUMP, FALSE);
    bakeSettings.SetBool(BAKE_TEX_TRANSPARENCY, FALSE);
    bakeSettings.SetBool(BAKE_TEX_UVMAP, FALSE);
    bakeSettings.SetBool(BAKE_TEX_REFLECTION, FALSE);

    bakeSettings.SetBool(BAKE_TEX_USE_PHONG_TAG, TRUE);
    bakeSettings.SetBool(BAKE_TEX_COLOR_ILLUM, TRUE);
    bakeSettings.SetBool(BAKE_TEX_COLOR_SHADOWS, TRUE);

    bakeSettings.SetInt32(BAKE_TEX_WIDTH, 512);
    bakeSettings.SetInt32(BAKE_TEX_HEIGHT, 512);
    bakeSettings.SetInt32(BAKE_TEX_PIXELBORDER, 2);
    bakeSettings.SetVector(BAKE_TEX_FILL_COLOR, Vector(1.0));

    BaseTag* textag = obj->GetTag(Ttexture);
    BaseTag* uvwtag = obj->GetTag(Tuvw);

    if(textag)
    {
        Int32 error;

        BaseDocument *bakeDoc = InitBakeTexture(
                                                doc,
                                                (TextureTag*)textag,
                                                (UVWTag*)uvwtag,
                                                NULL,
                                                NULL,
                                                bakeSettings,
                                                &error,
                                                NULL);
//        BaseDocument *bakeDoc = InitBakeTexture(doc, //The document.
//                                                &textags[0], //The texture tags to bake. Must be assigned to objects.
//                                                texuvws.GetFirst(), //The UVW tags to bake.
//                                                nullptr/*destuvws.GetFirst()*/,  //The destination UVW tags for the bake. If not nullptr, the current projection is transformed into the uvw tags.
//                                                textags.GetCount(), //The number of tags in textags, texuvws and destuvws arrays.
//                                                settingsBc, //The bake settings: BakeTexEnums
//                                                &bakeError //Assigned the error result, if not nullptr: BAKE_TEX_ERR
//                                                )

        AutoAlloc<BaseBitmap> bmp;
        if (!bmp) return FALSE;

        if (bakeDoc) error = BakeTexture(bakeDoc, bakeSettings, bmp, NULL, NULL, NULL);

        ShowBitmap(bmp);
    }

    return TRUE;
}

Bool RegisterMenuTest(void)
{
	// be sure to use a unique ID obtained from www.plugincafe.com
	return RegisterCommandPlugin(1000956, GeLoadString(IDS_MENUTEST), 0, AutoBitmap("icon.tif"), String("C++ SDK Menu Test Plugin"), NewObjClear(MenuTest));
}

