[![Build Status][travis-ci-build-image]][travis-ci-build-url]
# cxx-mime-types
The ultimate c++ header only content-type utility.

Inspired by  [the `mime-types@2.1.25` module](https://www.npmjs.com/package/mime-types) and uses the same
[mime-db](https://www.npmjs.com/package/mime-db) database.

# API

```
#include "mime_types.h"

    pmc::mimetypes mime("./node_modules/mime-db/db.json");

```

## mime.lookup(file)
Lookup the Content-Type associated with a file.

```
    std::string_view&& type=mime.lookup(std::string_view(std::string("index.html")));      // "text/html"
    std::string_view&& jstype=mime.lookup(std::string_view(std::string("myscript.js")));   // "application/javascript"
    std::string_view&& utype=mime.lookup(std::string_view(std::string("unknown")));        // ""

```

## License

[MIT](LICENSE)


[travis-ci-build-image]: https://travis-ci.org/DataDriven-CAM/cxx-mime-types.svg?branch=master
[travis-ci-build-url]: https://travis-ci.org/DataDriven-CAM/cxx-mime-types
