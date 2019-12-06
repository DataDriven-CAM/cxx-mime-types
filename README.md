# cxx-mime-types
The ultimate c++ header only content-type utility.


# API

```
    pmc::mimetypes mime("../node_modules/mime-db/db.json");

```

## mime.lookup(file)
Lookup the Content-Type associated with a file.

```
    std::string_view&& type=mimetypes.lookup(std::string_view(std::string("index.html")));      // "text/html"
    std::string_view&& jstype=mimetypes.lookup(std::string_view(std::string("myscript.js")));   // "application/javascript"
    std::string_view&& utype=mimetypes.lookup(std::string_view(std::string("unknown")));        // ""

```

## License

[MIT](LICENSE)