CONTAINER toolsculptdrawpolybrush
{
  NAME toolsculptdrawpolybrush;
  INCLUDE toolsculptbrushbase;
 
	GROUP MDATA_SCULPTBRUSH_SETTINGS_GROUP
	{
		COLUMNS 3;
	
		//Add all the drawmodes
		GROUP
		{
			COLUMNS 3;
			LONG MDATA_SCULPTBRUSH_SETTINGS_DRAWMODE
			{
				CYCLE
				{
					MDATA_SCULPTBRUSH_SETTINGS_DRAWMODE_FREEHAND;
					MDATA_SCULPTBRUSH_SETTINGS_DRAWMODE_LINE;
				}
			}
		}
		STATICTEXT { JOINENDSCALE; }
		STATICTEXT { JOINEND; }
	}
}
