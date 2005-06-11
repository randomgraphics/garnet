macro GN_HFileHeader()
{
	// let use input module name
	module = ask("Module name?")
	module = toupper(module)

	// username is always chenlee
	username = "chenlee"
	
	hbuf = GetCurrentBuf()
	fullpath = GetBufName( hbuf )
	date = GetSysTime(true)
	year = date.Year
	month = date.Month
	day = date.Day

	// decode file path

	base_start = 0
	base_end = 0
	ext_start = 0
	ext_end = 0

	len = strLen(fullpath);
	n = len
	while( n > 0 )
	{
		n = n - 1
		if ( fullpath[n] == "." )
		{
			break;
		}
	}
	ext_start = n+1
	ext_end = len
	while( n > 0 )
	{
		n = n - 1
		if ( fullpath[n] == "\\" )
		{
			break;
		}
	}
	base_start = n+1
	base_end = ext_start-1

	filebase = strmid( fullpath, base_start, base_end )
	fileext  = strmid( fullpath, ext_start, ext_end )
	filebase_up = toupper(filebase)
	fileext_up = toupper(fileext)

	if ( strLen(module) > 0 )
	{
		tag = "__GN_@module@_@filebase_up@_@fileext_up@__"
	}
	else
	{
		tag = "__GN_@filebase_up@_@fileext_up@__"
	}

	// insert header
	InsBufLine( hbuf,  0, "#ifndef @tag@" )
	InsBufLine( hbuf,  1, "#define @tag@" )
	InsBufLine( hbuf,  2, "// *****************************************************************************")
	InsBufLine( hbuf,  3, "//! \\file    @filebase@.@fileext@" )
	InsBufLine( hbuf,  4, "//! \\brief   ?" )
	InsBufLine( hbuf,  5, "//! \\author  @username@ (@year@.@month@.@day@)" )
	InsBufLine( hbuf,  6, "// *****************************************************************************" )
	InsBufLine( hbuf,  7, "" )
	InsBufLine( hbuf,  8, "" )
	InsBufLine( hbuf,  9, "// *****************************************************************************" )
	InsBufLine( hbuf, 10, "//                           End of @filebase@.@fileext@" )
	InsBufLine( hbuf, 11, "// *****************************************************************************" )
	InsBufLine( hbuf, 12, "#endif // @tag@" )

	// put the insertion point inside the header comment
	SetBufIns( hbuf, 4, 14 )
}

