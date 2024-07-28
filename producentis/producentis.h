#ifdef _WIN32
#ifdef producentis_EXPORTS
#define producentis_API __declspec(dllexport)
#else
#define producentis_API __declspec(dllimport)
#endif
#else
#define producentis_API
#endif
