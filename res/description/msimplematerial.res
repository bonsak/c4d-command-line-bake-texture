CONTAINER Msimplematerial
{
	NAME Msimplematerial;

	GROUP
	{
		MATPREVIEW SIMPLEMATERIAL_MAT_PREVIEW { OPEN; MIN_WIDTH 100; MIN_HEIGHT 100; NO_SIZE_POPUP; }
	}

	INCLUDE Mbase;

	GROUP Obaselist
	{
		BOOL MATERIAL_PAGE_PROPERTIES { HIDDEN; PAGE; PARENTMSG ID_MATERIALPROPERTIES; }
	}
	
	GROUP ID_MATERIALPROPERTIES
	{
		DEFAULT 1;
		COLOR 	SIMPLEMATERIAL_COLOR {}
	}

	INCLUDE Millum;
	INCLUDE Massign;
}
