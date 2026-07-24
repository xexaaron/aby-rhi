#include "interfaces/default_fileio.hpp"
#include <fstream>

namespace aby::rhi {

    auto DefaultFileIO::set_cwd(const fs::path& path) -> void {
        m_CWD = path;
    }

    auto DefaultFileIO::cwd() const -> const fs::path& {
        return m_CWD;
    }

    auto DefaultFileIO::read(const fs::path& rel_path, std::vector<uint8_t>* data) -> bool {
        std::ifstream ifs(m_CWD / rel_path, std::ios::binary | std::ios::ate); 
        if (!ifs.is_open()) return false;

        size_t bytes = static_cast<size_t>(ifs.tellg());
        data->resize(bytes);
        ifs.seekg(0, std::ios::beg);
        ifs.read(reinterpret_cast<char*>(data->data()), bytes);
        
        if (ifs.gcount() != bytes) return false;

        return true;
    }

    auto DefaultFileIO::read(const fs::path& rel_path, std::vector<uint32_t>* data) -> bool {
        std::ifstream ifs(m_CWD / rel_path, std::ios::binary | std::ios::ate); 
        if (!ifs.is_open()) return false;

        size_t bytes = static_cast<size_t>(ifs.tellg());
        data->resize(bytes / sizeof(uint32_t));
        ifs.seekg(0, std::ios::beg);
        ifs.read(reinterpret_cast<char*>(data->data()), bytes);
        
        if (ifs.gcount() != bytes) return false;

        return true;
    }

    auto DefaultFileIO::write(const fs::path& rel_path, std::span<uint8_t> data) -> bool {
        std::ofstream ofs(m_CWD / rel_path, std::ios::binary);
        if (!ofs.is_open()) return false;

        ofs.write(reinterpret_cast<const char*>(data.data()), data.size());
        if (!ofs.good()) {
            ofs.close();
            return false;
        }
       
        ofs.close();
        return true;
    }


}