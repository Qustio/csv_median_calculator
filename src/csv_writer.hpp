#ifndef CSV_WRITER
#define CSV_WRITER

#include <filesystem>
#include "rapidcsv.h"


namespace fs = std::filesystem;

class CsvWriter {
public:
    CsvWriter(const fs::path& output_path_);
    CsvWriter(const CsvWriter&) = delete;
    CsvWriter& operator=(const CsvWriter&) = delete;
    CsvWriter(CsvWriter&&) noexcept = default;
    CsvWriter& operator=(CsvWriter&&) noexcept = default;
    ~CsvWriter();

    void write_median_change(long long int timestamp, double median);
private:
    void write_header();
    rapidcsv::Document _file;
    fs::path _path;
    size_t _current_row = 0;
};

#endif // CSV_WRITER
