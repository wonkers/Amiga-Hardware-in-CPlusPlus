
class LibraryControl
{
    public:
    LibraryControl()
    {

    }
    struct ExecBase *SysBase;
    struct DosLibrary *DOSBase;
    struct GfxBase *GfxBase;

    bool OpenLibraries()
    {
        if((SysBase = *((struct ExecBase**)4UL)) != NULL)
        {
            if((GfxBase = (struct GfxBase *)OpenLibrary((CONST_STRPTR)"graphics.library",0))!=NULL)
            {
                if((DOSBase = (struct DosLibrary*)OpenLibrary((CONST_STRPTR)"dos.library", 0)) != NULL)
                {
                    return true;
                }
                else{
                    CloseLibrary((Library*)GfxBase);
                    CloseLibrary((Library*)SysBase);
                }
            }
            else{
                CloseLibrary((Library*)SysBase);
            }
        }

        return false;
    }
};