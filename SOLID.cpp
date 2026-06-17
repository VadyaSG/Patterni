#include <fstream>
#include <string>
#include <memory>

class HTMLPrintable 
{
public:
    virtual ~HTMLPrintable() = default;
    virtual std::string printAsHTML() const = 0;
};

class TextPrintable
{
public:
    virtual ~TextPrintable() = default;
    virtual std::string printAsText() const = 0;
};

class JSONPrintable 
{
public:
    virtual ~JSONPrintable() = default;
    virtual std::string printAsJSON() const = 0;
};

class Data 
{
public:
    explicit Data(std::string data) : data_(std::move(data)) {}
    virtual ~Data() = default;

    const std::string& getData() const { return data_; }

private:
    std::string data_;
};

class HTMLData : public Data, public HTMLPrintable
{
public:
    explicit HTMLData(std::string data) : Data(std::move(data)) {}

    std::string printAsHTML() const override 
    {
        return "<html>" + getData() + "</html>";
    }
};

class TextData : public Data, public TextPrintable 
{
public:
    explicit TextData(std::string data) : Data(std::move(data)) {}

    std::string printAsText() const override 
    {
        return getData();
    }
};

class JSONData : public Data, public JSONPrintable 
{
public:
    explicit JSONData(std::string data) : Data(std::move(data)) {}

    std::string printAsJSON() const override 
    {
        return "{ \"data\": \"" + getData() + "\" }";
    }
};

void saveToAsHTML(std::ofstream& file, const HTMLPrintable& printable) 
{
    file << printable.printAsHTML();
}

void saveToAsText(std::ofstream& file, const TextPrintable& printable) 
{
    file << printable.printAsText();
}

void saveToAsJSON(std::ofstream& file, const JSONPrintable& printable) 
{
    file << printable.printAsJSON();
}

template<typename T>
void saveTo(std::ofstream& file, const T& printable)
{
  
    if constexpr (std::is_base_of_v<HTMLPrintable, T>) 
    {
        saveToAsHTML(file, printable);
    }
    else if constexpr (std::is_base_of_v<TextPrintable, T>)
    {
        saveToAsText(file, printable);
    }
    else if constexpr (std::is_base_of_v<JSONPrintable, T>) 
    {
        saveToAsJSON(file, printable);
    }
}


int main() 
{
    

    return 0;
}
