macro GN_HFileHeader()
{
	// get username
	username = cat( cat( GetEnv( "USERNAME" ), "\@\@" ), GetEnv("USERDOMAIN") )

	hbuf = GetCurrentBuf()
	fullpath = GetBufName( hbuf )
	date = GetSysTime(true)
	year = date.Year
	month = date.Month
	day = date.Day

	// decode file path

	parent_start = 0;
	parent_end = 0;
	base_start = 0
	base_end = 0
	ext_start = 0
	ext_end = 0

	len = strlen(fullpath);
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
	while( n > 0 )
	{
		n = n - 1
		if ( fullpath[n] == "\\" )
		{
			break;
		}
	}
	parent_start = n + 1;
	parent_end = base_start - 1;

	filebase = toupper( strmid( fullpath, base_start, base_end ) )
	fileext  = toupper( strmid( fullpath, ext_start, ext_end ) )

	if( "PCH" == filebase && "H" == fileext )
	{
		tag = "__GN_PCH_H__"
		brief = "PCH header"
		doxygen = ""
	}
	else
	{
		// let user input module name
		module = ask("Module name?")
		module = toupper(module)
		tag = "__GN_@module@_@filebase@_@fileext@__"
		brief = "?"
		doxygen = "/"
	}

	// insert header
	InsBufLine( hbuf,  0, "#ifndef @tag@" )
	InsBufLine( hbuf,  1, "#define @tag@" )
	InsBufLine( hbuf,  2, "// *****************************************************************************")
	InsBufLine( hbuf,  3, "//@doxygen@ \\file" )
	InsBufLine( hbuf,  4, "//@doxygen@ \\brief   @brief@" )
	InsBufLine( hbuf,  5, "//@doxygen@ \\author  @username@ (@year@.@month@.@day@)" )
	InsBufLine( hbuf,  6, "// *****************************************************************************" )
	InsBufLine( hbuf,  7, "" )
	InsBufLine( hbuf,  8, "" )
	InsBufLine( hbuf,  9, "// *****************************************************************************" )
	InsBufLine( hbuf, 10, "//                                     EOF" )
	InsBufLine( hbuf, 11, "// *****************************************************************************" )
	InsBufLine( hbuf, 12, "#endif // @tag@" )

	// put the insertion point inside the header comment
	SetBufIns( hbuf, 4, 14 )
}

macro GN_StdClassDecl()
{
	// let user input parent name
	parentName = ask("Parent class name?")

	hbuf = GetCurrentBuf()
	className = GetBufSelText(hbuf)

	currentLine = GetBufLnCur(hbuf)

	SetBufSelText( hbuf, "" )

	InsBufLine( hbuf,  currentLine+ 0, "///" )
	InsBufLine( hbuf,  currentLine+ 1, "/// " )
	InsBufLine( hbuf,  currentLine+ 2, "///" )
	InsBufLine( hbuf,  currentLine+ 3, "class @className@ : public @parentName@" )
	InsBufLine( hbuf,  currentLine+ 4, "{" )
	InsBufLine( hbuf,  currentLine+ 5, "    GN_DECLARE_STDCLASS( @className@, @parentName@ );" )
    InsBufLine( hbuf,  currentLine+ 6, "" )
	InsBufLine( hbuf,  currentLine+ 7, "    // ********************************" )
	InsBufLine( hbuf,  currentLine+ 8, "    // ctor/dtor" )
	InsBufLine( hbuf,  currentLine+ 9, "    // ********************************" )
    InsBufLine( hbuf,  currentLine+10, "" )
	InsBufLine( hbuf,  currentLine+11, "    //\@{" )
	InsBufLine( hbuf,  currentLine+12, "public:" )
	InsBufLine( hbuf,  currentLine+13, "    @className@()          { clear(); }" )
	InsBufLine( hbuf,  currentLine+14, "    virtual ~@className@() { quit(); }" )
	InsBufLine( hbuf,  currentLine+15, "    //\@}" )
    InsBufLine( hbuf,  currentLine+16, "" )
	InsBufLine( hbuf,  currentLine+17, "    // ********************************" )
	InsBufLine( hbuf,  currentLine+18, "    // from StdClass" )
	InsBufLine( hbuf,  currentLine+19, "    // ********************************" )
    InsBufLine( hbuf,  currentLine+20, "" )
	InsBufLine( hbuf,  currentLine+21, "    //\@{" )
	InsBufLine( hbuf,  currentLine+22, "public:" )
	InsBufLine( hbuf,  currentLine+23, "    bool init();" )
	InsBufLine( hbuf,  currentLine+24, "    void quit();" )
	InsBufLine( hbuf,  currentLine+25, "private:" )
	InsBufLine( hbuf,  currentLine+26, "    void clear() {}" )
	InsBufLine( hbuf,  currentLine+27, "    //\@}" )
    InsBufLine( hbuf,  currentLine+28, "" )
	InsBufLine( hbuf,  currentLine+29, "    // ********************************" )
	InsBufLine( hbuf,  currentLine+30, "    // public functions" )
	InsBufLine( hbuf,  currentLine+31, "    // ********************************" )
	InsBufLine( hbuf,  currentLine+32, "public:" )
    InsBufLine( hbuf,  currentLine+33, "" )
	InsBufLine( hbuf,  currentLine+34, "    // ********************************" )
	InsBufLine( hbuf,  currentLine+35, "    // private variables" )
	InsBufLine( hbuf,  currentLine+36, "    // ********************************" )
	InsBufLine( hbuf,  currentLine+37, "private:" )
    InsBufLine( hbuf,  currentLine+38, "" )
	InsBufLine( hbuf,  currentLine+39, "    // ********************************" )
	InsBufLine( hbuf,  currentLine+40, "    // private functions" )
	InsBufLine( hbuf,  currentLine+41, "    // ********************************" )
	InsBufLine( hbuf,  currentLine+42, "private:" )
	InsBufLine( hbuf,  currentLine+43, "};" )

	// put the insertion point inside the header comment
	SetBufIns( hbuf, currentLine+1, 4 )
}


macro GN_StdClassImpl()
{
	hbuf = GetCurrentBuf()
	className = GetBufSelText(hbuf)

	currentLine = GetBufLnCur(hbuf)

	SetBufSelText( hbuf, "" )

    InsBufLine( hbuf,  currentLine+ 0, "// *****************************************************************************" )
    InsBufLine( hbuf,  currentLine+ 1, "// @className@ - Initialize and shutdown" )
    InsBufLine( hbuf,  currentLine+ 2, "// *****************************************************************************" )
    InsBufLine( hbuf,  currentLine+ 3, "" )
    InsBufLine( hbuf,  currentLine+ 4, "//" )
    InsBufLine( hbuf,  currentLine+ 5, "//" )
    InsBufLine( hbuf,  currentLine+ 6, "// -----------------------------------------------------------------------------" )
    InsBufLine( hbuf,  currentLine+ 7, "bool @className@::init()" )
    InsBufLine( hbuf,  currentLine+ 8, "{" )
    InsBufLine( hbuf,  currentLine+ 9, "    GN_GUARD;" )
    InsBufLine( hbuf,  currentLine+10, "" )
    InsBufLine( hbuf,  currentLine+11, "    // standard init procedure" )
    InsBufLine( hbuf,  currentLine+12, "    GN_STDCLASS_INIT( @className@, () );" )
    InsBufLine( hbuf,  currentLine+13, "" )
    InsBufLine( hbuf,  currentLine+14, "    // Do custom init here" )
    InsBufLine( hbuf,  currentLine+15, "" )
    InsBufLine( hbuf,  currentLine+16, "    // success" )
    InsBufLine( hbuf,  currentLine+17, "    return success();" )
    InsBufLine( hbuf,  currentLine+18, "" )
    InsBufLine( hbuf,  currentLine+19, "    GN_UNGUARD;" )
    InsBufLine( hbuf,  currentLine+20, "}" )
    InsBufLine( hbuf,  currentLine+21, "" )
    InsBufLine( hbuf,  currentLine+22, "//" )
    InsBufLine( hbuf,  currentLine+23, "//" )
    InsBufLine( hbuf,  currentLine+24, "// -----------------------------------------------------------------------------" )
    InsBufLine( hbuf,  currentLine+25, "void @className@::quit()" )
    InsBufLine( hbuf,  currentLine+26, "{" )
    InsBufLine( hbuf,  currentLine+27, "    GN_GUARD;" )
    InsBufLine( hbuf,  currentLine+28, "" )
    InsBufLine( hbuf,  currentLine+29, "    // Do custom shutting down here" )
    InsBufLine( hbuf,  currentLine+30, "" )
    InsBufLine( hbuf,  currentLine+31, "    // standard quit procedure" )
    InsBufLine( hbuf,  currentLine+32, "    GN_STDCLASS_QUIT();" )
    InsBufLine( hbuf,  currentLine+33, "" )
    InsBufLine( hbuf,  currentLine+34, "    GN_UNGUARD;" )
    InsBufLine( hbuf,  currentLine+35, "}" )
}
