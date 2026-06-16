#include <string>
#include <iostream>
#include <algorithm>

class Text 
{
public:
    virtual void render(const std::string& data) const 
    {
        std::cout << data;
    }
};


class DecoratedText : public Text
{
public:
    explicit DecoratedText(Text* text) : text_(text) {}
    Text* text_;
};


class ItalicText : public DecoratedText {
public:
    explicit ItalicText(Text* text) : DecoratedText(text) {}
    void render(const std::string& data)  const 
    {
        std::cout << "<i>";
        text_->render(data);
        std::cout << "</i>";
    }
};

class BoldText : public DecoratedText 
{
public:
    explicit BoldText(Text* text) : DecoratedText(text) {}
    void render(const std::string& data) const 
    {
        std::cout << "<b>";
        text_->render(data);
        std::cout << "</b>";
    }
};

class Paragraph : public DecoratedText
{
public:
    explicit Paragraph(Text* text) :DecoratedText(text){}
    void render(const std::string& data)const
    {
        std::cout << "<p>";
        text_->render(data);
        std::cout << "</p>";
    }
};

class Reversed : public DecoratedText
{
public:
    explicit Reversed(Text* text) :DecoratedText(text) {}
    void render(const std::string& data)const override
    {
        std::string reversed = data;
        std::reverse(reversed.begin(), reversed.end());
        text_->render(reversed);
    }
};

class Link : public DecoratedText
{
public:
    explicit Link(Text* text, const std::string& url) : DecoratedText(text), url_(url)   {}

    void render(const std::string& data) const override
    {
        std::cout << "<a href=" << url_ << ">";
        text_->render(data);
        std::cout << "</a>";
    }

private:
    std::string url_;
};

int main() 
{
    auto text_block = new ItalicText(new BoldText(new Text()));
    text_block->render("Hello world");
    std::cout << std::endl;

    auto text_block1 = new Paragraph (new Text());
    text_block1->render("Hello world");
    std::cout << std::endl;

    auto text_block2 = new Reversed(new Text());
    text_block2->render("Hello world");
    std::cout << std::endl;

    auto text_block3 = new Link(new Text(), "netology.ru");
    text_block3->render("Hello world");
    std::cout << std::endl;
}