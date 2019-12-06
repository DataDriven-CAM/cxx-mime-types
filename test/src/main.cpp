#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <exception>
#include <memory>
#include <string>

#include "mime_types.h"

namespace pmc{
class Exception : public std::exception {
public:
    std::string message;
public:
    Exception()
    {
    }
    Exception(std::string message)
    {
        this->message=message;
    }
    Exception(const Exception& orig)
    {
    }
    virtual ~Exception() throw()
    {
    }
    virtual const char* what(){return message.c_str();};

protected:

};

    
}

TEST_CASE("testing the mime types lookup function") {
//  try
//  {
    pmc::mimetypes mimetypes("../node_modules/mime-db/db.json");
    std::string_view&& type=mimetypes.lookup(std::string_view(std::string("index.html")));
    CHECK(type.compare("text/html")==0);
    std::string_view&& jstype=mimetypes.lookup(std::string_view(std::string("myscript.js")));
    CHECK(jstype.compare("application/javascript")==0);
    std::string_view&& utype=mimetypes.lookup(std::string_view(std::string("unknown")));
    CHECK(utype.compare("application/javascript")!=0);
//  }
//  catch (pmc::Exception& e)
//  {
//    std::cout <<"Exception\n"<< e.message << std::endl;
//    //return EXIT_FAILURE;
//  }
//  catch (std::exception& e)
//  {
//    std::cout << e.what() << std::endl;
//    //return EXIT_FAILURE;
//  }
}
