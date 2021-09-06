#include "svg/tests.h"

#include <fstream>

namespace html
{
  void begin(std::ostream& o)
  {
    o << "<!doctype html>\n";
    o << "<html lang=\"en\">\n";
    o << "\n";
    o << "<head>\n";
    o << "  <meta charset=\"utf-8\">\n";
    o << "  <meta name=\"viewport\" content=\"width=device-width, minimum-scale=1, initial-scale=1, user-scalable=yes\">\n";
    o << "  <title> .: rough tests :. </title>\n";
    o << "</head>\n";
    o << "\n";
    o << "<body>\n";
  }

  void end(std::ostream& o)
  {
      o << "</body>\n";
      o << "</html>\n";      
  }
};

const std::string INDENT = "    ";

void write_svg_element(std::ostream& o, const std::shared_ptr<svg::Element>& el, const std::string& space)
{
  o << space << "<" << el->name << " ";

  for(const auto& [att, val]: el->attributes)
  {
    o << att << "=" << '\"' << val << "\" ";
  }

  if(el->children.empty())
  {
    o << "/>\n";
  }
  else
  {
    o << ">\n";

    for(const auto& ch: el->children)
    {
      write_svg_element(o, ch, space+INDENT);
    }

    o << space << "</" << el->name << ">\n";
  }
}

void write_svg(std::ostream& o, const svg::Document& svg)
{
  o << "<svg width=\"800\" height=\"800\" xmlns=\"http://www.w3.org/2000/svg\">\n";

  for(const auto& el: svg.elements)
  {
    write_svg_element(o, el, INDENT);
  }

  o << "</svg>\n";
}

struct TestRunner
{
    bool split_svg_tests = false;

    std::optional<std::fstream> file;

    void add(const std::string& name, const svg::Document& document)
    {
        if (split_svg_tests)
        {
            std::string filename = name + ".svg";
            file.emplace(filename, std::ios::out);
            if (file->is_open())
            {
                write_svg(*file, document);
                file->close();
            }
        }
        else
        {
            if(!file)
            {
                std::string filename = "tests.html";
                file.emplace(filename, std::ios::out);

                html::begin(*file);
            }

            if(file && file->is_open())
            {
                *file << "<div class=\"test\">\n";
                *file << "<h2>" << name << "</h2>\n";

                *file << "<div class=\"svg\">\n";
                write_svg(*file, document);
                *file << "</div>\n";
            }
        }
    }

    ~TestRunner()
    {
        if(file && file->is_open())
        {
            html::end(*file);
            file->close();
            file = {};
        }
    }
};

void run_tests(TestRunner* runner)
{
    runner->add("rectangle", rectangle_test());
    runner->add("polygon", polygon_test());
    runner->add("ellipse", ellipse_test());
    runner->add("line", line_test());
}

int main(int argc, char** argv)
{
    bool split_svg_tests = false;

    for(int i=1; i<argc; i++)
    {
        if(strcmp(argv[i], "--split") == 0)
        {
            split_svg_tests = true;
        }
    }

    TestRunner runner;
    runner.split_svg_tests = split_svg_tests;
    run_tests(&runner);

    return 0;
}
