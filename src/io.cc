#include <cstdio>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>

#include "io.hh"
#include "utils.hh"

namespace io {
  File::File(FILE *fd, std::string fname, std::string md) {
    f = fd;
    filename.assign(fname);
    md.assign(md);
  }

  File::~File(void) { fclose(f); }

  File *Open(std::string filename, std::string mode) {
    FILE *fd = fopen(filename.c_str(), mode.c_str());
    if (fd == NULL)
      utils::PrintfError("Could not open file [%s] with mode [%s].\n",
          filename.c_str(), mode.c_str());
    File *f = new File(fd, filename, mode);
    return f;
  }

  void File::Open(void) {
    if (f == NULL)
      f = fopen(filename.c_str(), mode.c_str());
  }

  void File::Close(void) {
    fclose(f);
    f = NULL;
  }

  void Close(File *f) {
    delete f;
  }

  void Write(File *f, const std::string &text) {
    fprintf(f->f, "%s", text.c_str());
  }

  void Clear(File *f) {
    freopen(NULL, "w", f->f);
    freopen(NULL, f->mode.c_str(), f->f);
  }

  void Mkdir(std::string dir) {
    const char *p = dir.c_str();
    struct stat st{};
    if (stat(p, &st))
      if (mkdir(p, 0700))
        utils::PrintfError("Could not create directory [%s].", p);
  }
}
