#include "Font.hpp"
#include "../External/glad/glad.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "../External/stb/stb_truetype.h"
// #include <ft2build.h>
// #include FT_FREETYPE_H
#include <iostream>
#include <fstream>

// namespace GravyEngine 
// {
//     std::unordered_map<std::string,Font> Font::fonts;

//     Font::Font() :
//         textureId(0), 
//         pixelSize(0), 
//         maxHeight(0),
//         lineHeight(0),
//         codePointOfFirstChar(0) 
//     {
//     }

//     Font::Font(const Font &other) : 
//         pixelSize(other.pixelSize), 
//         textureId(other.textureId), 
//         maxHeight(other.maxHeight),
//         lineHeight(other.lineHeight),
//         codePointOfFirstChar(other.codePointOfFirstChar),
//         glyphs(other.glyphs),
//         textureData(other.textureData) 
//     {
//     }

//     Font::Font(Font &&other) noexcept 
//     {
//         pixelSize = other.pixelSize;
//         textureId = other.textureId;
//         maxHeight = other.maxHeight;
//         lineHeight = other.lineHeight;
//         codePointOfFirstChar = other.codePointOfFirstChar;
//         glyphs = std::move(other.glyphs);
//         textureData = std::move(other.textureData);
//     }

//     Font &Font::operator=(const Font &other) 
//     {
//         if(this != &other) 
//         {
//             pixelSize = other.pixelSize;
//             textureId = other.textureId;
//             maxHeight = other.maxHeight;
//             lineHeight = other.lineHeight;
//             codePointOfFirstChar = other.codePointOfFirstChar;
//             glyphs = other.glyphs;
//             textureData = other.textureData;
//         }
//         return *this;
//     }

//     Font &Font::operator=(Font &&other) noexcept 
//     {
//         if(this != &other) 
//         {
//             pixelSize = other.pixelSize;
//             textureId = other.textureId;
//             maxHeight = other.maxHeight;
//             lineHeight = other.lineHeight;
//             codePointOfFirstChar = other.codePointOfFirstChar;
//             glyphs = std::move(other.glyphs);
//             textureData = std::move(other.textureData);
//         }
//         return *this;
//     }

//     bool Font::LoadFromFile(const std::string &filepath, uint32_t pixelSize, FontMode fontMode, bool createTexture) 
//     {
//         if(textureData.size() > 0) 
//         {
//             std::cerr << "Could not load font because texture is already generated\n";
//             return false;
//         }

//         this->pixelSize = pixelSize;

//         FT_Library library;
//         if (FT_Init_FreeType(&library)) 
//         {
//             std::cerr << "Could not init FreeType library\n";
//             return false;
//         }

//         FT_Face fontFace;
//         if (FT_New_Face(library, filepath.c_str(), 0, &fontFace)) 
//         {
//             FT_Done_FreeType(library);
//             std::cerr << "Could not load font\n";
//             return false;
//         }

//         bool result = Load(fontFace, fontMode);

//         FT_Done_Face(fontFace);
//         FT_Done_FreeType(library);

//         if(result && createTexture) 
//         {
//             GenerateTexture();
//         }

//         return result;
//     }

//     bool Font::LoadFromMemory(const void *data, size_t dataSize, uint32_t pixelSize, FontMode fontMode, bool createTexture) 
//     {
//         if(textureData.size() > 0) 
//         {
//             std::cerr << "Could not load font because texture is already generated\n";
//             return false;
//         }

//         this->pixelSize = pixelSize;

//         FT_Library library;
//         if (FT_Init_FreeType(&library)) 
//         {
//             std::cerr << "Could not init FreeType library\n";
//             return false;
//         }

//         FT_Face fontFace;
//         const unsigned char *pData = reinterpret_cast<const FT_Byte*>(data);
//         if (FT_New_Memory_Face(library, pData, dataSize, 0, &fontFace)) 
//         {
//             FT_Done_FreeType(library);
//             std::cerr << "Could not load font\n";
//             return false;
//         }

//         bool result = Load(fontFace, fontMode);

//         FT_Done_Face(fontFace);
//         FT_Done_FreeType(library);

//         if(result && createTexture) 
//         {
//             GenerateTexture();
//         }

//         return result;
//     }

//     bool Font::Load(FT_Face fontFace, FontMode fontMode) 
//     {
//         FT_Set_Pixel_Sizes(fontFace, 0, pixelSize);
        
//         const uint32_t textureWidth = 1024;
//         const uint32_t textureHeight = 1024;
//         const uint32_t padding = 2;
//         int row = 0;
//         int col = 0;
//         int64_t height = 0;

//         FT_Int32 loadFlags = FT_LOAD_RENDER;

//         if(fontMode == FontMode::SDF)
//             loadFlags |= FT_LOAD_TARGET_(FT_RENDER_MODE_SDF);
//         else
//             loadFlags |= FT_LOAD_TARGET_(FT_RENDER_MODE_NORMAL);

//         textureData.resize(textureWidth * textureHeight);

//         codePointOfFirstChar = 32;
//         const uint32_t charsToIncludeInFontAtlas = 95;
//         const FT_ULong start = static_cast<FT_ULong>(codePointOfFirstChar);
//         const FT_ULong end = start + static_cast<FT_ULong>(charsToIncludeInFontAtlas);
//         glyphs.resize(charsToIncludeInFontAtlas);
//         size_t index = 0;
//         int64_t lineHeight = 0;
//         size_t maxRowHeight = 0;

//         //for(FT_ULong glyphIdx = 0; glyphIdx < numGlyphs; glyphIdx++) {
//         for(FT_ULong glyphIdx = start; glyphIdx < end; glyphIdx++) 
//         {
//             if(FT_Load_Char(fontFace, glyphIdx, loadFlags))
//                 continue;

//             if(FT_Render_Glyph(fontFace->glyph, FT_RENDER_MODE_NORMAL))
//                 continue;

//             if(fontFace->glyph->bitmap.rows > maxRowHeight)
//                 maxRowHeight = fontFace->glyph->bitmap.rows;

//             if(col + fontFace->glyph->bitmap.width + padding >= textureWidth) 
//             {
//                 col = padding;
//                 row += maxRowHeight + padding;
//                 maxRowHeight = 0;
//             }

//             maxHeight = (uint32_t)std::max((int64_t)(fontFace->size->metrics.ascender - fontFace->size->metrics.descender) >> 6, height);

//             for(FT_UInt y = 0; y < fontFace->glyph->bitmap.rows; y++) 
//             {
//                 for(FT_UInt x = 0; x < fontFace->glyph->bitmap.width; x++) 
//                 {
//                     FT_UInt indexA = (row + y) * textureWidth + col + x;
//                     FT_UInt indexB = y * fontFace->glyph->bitmap.width + x;
//                     textureData[indexA] = fontFace->glyph->bitmap.buffer[indexB];
//                 }
//             }

//             Glyph* glyph = &glyphs[index++];
//             glyph->sizeX = fontFace->glyph->bitmap.width;
//             glyph->sizeY = fontFace->glyph->bitmap.rows;
//             glyph->advanceX = fontFace->glyph->advance.x >> 6;
//             glyph->advanceY = fontFace->glyph->advance.y >> 6;
//             glyph->bearingX = fontFace->glyph->bitmap_left;
//             glyph->bearingY = fontFace->glyph->bitmap_top;
//             glyph->height = fontFace->glyph->metrics.height >> 6;
//             glyph->bottomBearing = (fontFace->glyph->bitmap.rows - fontFace->glyph->bitmap_top);
//             glyph->leftBearing = (fontFace->glyph->bitmap.width - fontFace->glyph->bitmap_left);

//             lineHeight = std::max(glyph->height, lineHeight);

//             glyph->u0 = static_cast<float>(col) / textureWidth;
//             glyph->v0 = static_cast<float>(row) / textureHeight;
//             glyph->u1 = static_cast<float>(col + fontFace->glyph->bitmap.width) / textureWidth;
//             glyph->v1 = static_cast<float>(row + fontFace->glyph->bitmap.rows) / textureHeight;

//             col += fontFace->glyph->bitmap.width + padding;
//         }

//         this->lineHeight = static_cast<uint32_t>(lineHeight);

//         return true;
//     }

//     bool Font::GenerateTexture() 
//     {
//         if(textureId > 0) 
//         {
//             std::cerr << "Could not generate texture because it is already generated\n";
//             return false;
//         }

//         const uint32_t textureWidth = 1024;
//         const uint32_t textureHeight = 1024;

//         glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

//         glGenTextures(1, &textureId);
//         glBindTexture(GL_TEXTURE_2D, textureId);
//         glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, textureWidth, textureHeight, 0, GL_RED, GL_UNSIGNED_BYTE, textureData.data());
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//         glGenerateMipmap(GL_TEXTURE_2D);
//         glBindTexture(GL_TEXTURE_2D, 0);

//         //Image::saveAsPNG("test.png", textureData.data(), textureData.size(), textureWidth, textureHeight, 1);

//         return textureId > 0;
//     }

//     void Font::Destroy() 
//     {
//         if(textureId > 0) 
//         {
//             glDeleteTextures(1, &textureId);
//             textureId = 0;
//         }
//         if(textureData.size() > 0) 
//         {
//             textureData.clear();
//         }
//     }

//     uint32_t Font::GetPixelSize() const 
//     {
//         return pixelSize;
//     }

//     uint32_t Font::GetMaxHeight() const 
//     {
//         return maxHeight;
//     }

//     uint32_t Font::GetTexture() const 
//     {
//         return textureId;
//     }

//     Glyph *Font::GetGlyph(char c) 
//     {
//         uint32_t index = (c - codePointOfFirstChar);
//         if(index >= glyphs.size())
//             return nullptr;
//         return &glyphs[index];
//     }

//     uint32_t Font::GetCodePointOfFirstChar() const 
//     {
//         return codePointOfFirstChar;
//     }

//     void Font::CalculateBounds(const char *text, size_t size, float fontSize, float &width, float &height) 
//     {
//         width = 0;
//         height = 0;

//         int32_t maxHeight = 0; // Maximum height of any character
//         int32_t currentLineWidth = 0; // Width of the current line
//         int32_t currentHeight = 0;
//         int32_t lineCount = 1; // Count of lines

//         for(size_t i = 0; i < size; i++) 
//         {
//             char c = text[i];

//             if (c == '\n') 
//             {
//                 // End of a line
//                 if (currentLineWidth > width) 
//                 {
//                     width = static_cast<float>(currentLineWidth);
//                 }
//                 currentHeight += GetMaxHeight();

//                 currentLineWidth = 0; // Reset for the next line
//                 lineCount++; // Increment line count
//                 continue;
//             }

//             Glyph *glyph = GetGlyph(c);
//             if (!glyph)
//                 continue;

//             // Accumulate the width using the advanceX of the glyph
//             currentLineWidth += glyph->advanceX;

//             // Update the maximum height based on the glyph's height
//             int32_t glyphHeight = glyph->height;
//             if (glyphHeight > maxHeight) 
//             {
//                 maxHeight = glyphHeight;
//             }
//         }

//         // Check the last line
//         if (currentLineWidth > width) 
//         {
//             width = static_cast<float>(currentLineWidth);
//         }

//         if(lineCount > 1) 
//         {
//             height = currentHeight + maxHeight;
//         } 
//         else 
//         {
//             height = maxHeight;
//         }

//         width *= GetPixelScale(fontSize);
//         height *= GetPixelScale(fontSize);
//     }

//     void Font::CalculateCharacterPosition(const char *text, size_t size, size_t characterIndex, float fontSize, float &x, float &y) 
//     {
//         if(size == 0)
//             return;

//         float startPosX = x;
//         float startPosY = y;
//         float characterPosX = x;
//         float characterPosY = y;

//         // Calculate the character position based on the character index
//         for (int i = 0; i < characterIndex; ++i) 
//         {
//             char ch = text[i];

//             // Handle line breaks
//             if (ch == '\n') 
//             {
//                 characterPosX = startPosX; // Reset X position for a new line
//                 characterPosY += GetMaxHeight() * GetPixelScale(fontSize);
//                 continue;
//             }

//             Glyph *glyph = GetGlyph(ch);
//             if(!glyph)
//                 continue;

//             // Update the character position based on the glyph's x advance
//             characterPosX += glyph->advanceX * GetPixelScale(fontSize);
//         }

//         x = characterPosX;
//         y = characterPosY;
//     }

//     float Font::CalculateYOffset(const char *text, size_t size, float fontSize) 
//     {
//         float height = 0.0f;
//         float yOffset = 0.0f;
        
//         for(size_t i = 0; i < glyphs.size(); i++) 
//         {
//             if(text[i] == '\n')
//                 break;

//             Glyph *glyph = &glyphs[i];
            
//             if(!glyph)
//                 continue;            

//             float h = glyph->bearingY;

//             if(h > height) 
//             {
//                 height = h;
//                 yOffset = (glyph->bearingY - glyph->bottomBearing) * GetPixelScale(fontSize);
//             }
//         }

//         return yOffset;
//     }

//     float Font::GetPixelScale(float fontSize) const 
//     {
//         return fontSize / pixelSize;
//     }

//     float Font::GetLineHeight() const 
//     {
//         return (float)lineHeight;
//     }

//     Font *Font::Add(const std::string &name, const Font &font) 
//     {
//         if(fonts.count(name) > 0)
//             return nullptr;
//         fonts[name] = font;
//         return &fonts[name];
//     }

//     Font *Font::Find(const std::string &name) 
//     {
//         if(fonts.count(name) == 0)
//             return nullptr;
//         return &fonts[name];
//     }

//     void Font::Remove(const std::string &name, const Font &font) 
//     {
//         if(fonts.count(name) == 0)
//             return;
//         fonts[name].Destroy();
//         fonts.erase(name);
//     }
// }

namespace GravyEngine
{
std::unordered_map<std::string, Font> Font::fonts;

    Font::Font() {
        texture = 0;
        pixelSize = 14;
        textureWidth = 512;
        textureHeight = 512;
    }

    Font::Font(const std::string &filepath, uint32_t pixelSize) {
        this->pixelSize = pixelSize;
        texture = 0;
        loadFromFile(filepath);
    }

    Font::Font(const uint8_t *fontData, uint32_t pixelSize) {
        this->pixelSize = pixelSize;
        texture = 0;
        loadFromMemory(fontData);
    }

    Font::Font(const Font &other) {
        this->fontInfo = fontInfo;
        this->pixelSize = pixelSize;
        this->texture = texture;
        this->codePointOfFirstChar = codePointOfFirstChar;
        this->textureWidth = textureWidth;
        this->textureHeight = textureHeight;
        this->lineHeight = lineHeight;
        this->characters = other.characters;
        this->quads = other.quads;
    }

    void Font::destroy() {
        if(texture > 0) {
            glDeleteTextures(1, &texture);
            texture = 0;
        }
    }

    float Font::computeLineHeight(const std::string &text, float fontSize) {
        if(text.size() == 0)
            return 0;

        float height = 0;

        for(auto c : text) {
            if(c == '\n') {
                return height;
            }
            
            int index = c - codePointOfFirstChar;
            
            if(index >= characters.size())
                continue;
            
            auto glyph = &characters[index];

            float glyphHeight = (glyph->y1 - glyph->y0);
            
            if (glyphHeight > height) {
                height = glyphHeight;
            }
        }

        return height * (fontSize / getPixelSize());
    }

    float Font::computeTextWidth(const std::string &text, float fontSize) {
        if(text.size() == 0)
            return 0;
        
        float maxWidth = 0.0f;
        float currentWidth = 0.0f;
        const uint32_t codePointOfFirstChar = getCodePointOfFirstChar();
        const auto &packedChars = getCharacters();

        for (char ch : text) {
            if (ch == '\n') {
                maxWidth = std::max(maxWidth, currentWidth);
                currentWidth = 0.0f;
            } else {
                int glyphIndex = (ch - codePointOfFirstChar);
                if (glyphIndex >= 0 && glyphIndex < getNumberOfCharacters()) {
                    auto packedChar = &packedChars[glyphIndex];
                    currentWidth += packedChar->xadvance;
                }
            }
        }

        maxWidth = std::max(maxWidth, currentWidth); // Check last line
        return maxWidth * (fontSize / getPixelSize());
    }

    void Font::computeCursorPosition(const std::string &text, size_t cursorIndex, float fontSize, float &x, float &y) {
        if(text.size() == 0)
            return;

        fontSize = fontSize / getPixelSize();
        float startPosX = x;
        float startPosY = y;
        float cursorPosX = x;
        float cursorPosY = y;

        const uint32_t codePointOfFirstChar = getCodePointOfFirstChar();
        auto &packedChars = getCharacters();

        // Calculate the cursor position based on the cursorIndex
        for (int i = 0; i < cursorIndex; ++i) {
            char ch = text[i];

            // Handle line breaks
            if (ch == '\n') {
                cursorPosX = startPosX; // Reset X position for a new line
                cursorPosY += getLineHeight() * fontSize;
                continue;
            }

            int glyphIndex = (ch - codePointOfFirstChar);
            // Skip unknown glyphs
            if (glyphIndex < 0 || glyphIndex >= getNumberOfCharacters()) {
                continue;
            }

            auto packedChar = &packedChars[glyphIndex];

            // Update the cursor position based on the glyph's xadvance
            cursorPosX += packedChar->xadvance * fontSize;
        }

        x = cursorPosX;
        y = cursorPosY;
    }

    float Font::computeTextHeight(const std::string &text, float fontSize) {
        int lineCount = 0;

        for(char ch : text) {
            if(ch == '\n') {
                lineCount++;
                continue;
            }
        }

        //If no new line characters exist, return default value
        if(lineCount == 0)
            return getLineHeight() * (fontSize / getPixelSize());

        return lineCount * (getLineHeight() * (fontSize / getPixelSize()));
    }

    float Font::getStringWidth(const std::string &text, uint32_t from, uint32_t to, float fontSize) {
        int width = 0;

        int pCodePoint = 0;
        int pAdvancedWidth = 0;
        int pLeftSideBearing = 0;

        int i = from;
        while (i < to) {
            i += getCodePoint(text, to, i, pCodePoint);
            int cp = pCodePoint;

            stbtt_GetCodepointHMetrics(&fontInfo, cp, &pAdvancedWidth, &pLeftSideBearing);
            width += pAdvancedWidth;
        }

        fontSize = fontSize / getPixelSize();

        return width * stbtt_ScaleForPixelHeight(&fontInfo, fontSize);
    }

    int Font::getCodePoint(const std::string &text, int to, int i, int32_t &cpOut) {
        char32_t c1 = text[i];
        if (isHighSurrogate(c1) && i + 1 < to) {
            char32_t c2 = text[i + 1];
            if (isLowSurrogate(c2)) {
                cpOut = (c1 << 10) + c2 - 0x35FDC00; // Combining high and low surrogate to get code point
                return 2;
            }
        }
        cpOut = c1;
        return 1;
    }

    bool Font::isHighSurrogate(char32_t c) {
        return (c >= 0xD800 && c <= 0xDBFF);
    }

    bool Font::isLowSurrogate(char32_t c) {
        return (c >= 0xDC00 && c <= 0xDFFF);
    }

    bool Font::loadFromFile(const std::string &filepath) {
        std::ifstream inputStream(filepath.c_str(), std::ios::binary | std::ios::ate);
        if (!inputStream) {
            std::cerr << "Failed to open font file\n";
            return false;
        }

        std::streamsize fontFileSize = inputStream.tellg();
        inputStream.seekg(0, std::ios::beg);

        std::vector<uint8_t> fontDataBuf;
        fontDataBuf.resize(fontFileSize);

        inputStream.read((char*)fontDataBuf.data(), fontFileSize);

        uint8_t *buffer = fontDataBuf.data();

        return load(fontDataBuf.data());
    }

    bool Font::loadFromMemory(const uint8_t *fontData) {
        if(!fontData) {
            std::cerr << "Failed to read font data because it is null\n";
            return false;
        }
        return load(fontData);
    }

    //bool Font::load(const std::string &filepath) {
    bool Font::load(const uint8_t *data) {
        uint32_t fontCount = stbtt_GetNumberOfFonts(data);

        if(!stbtt_InitFont(&fontInfo, data, 0)) {
            std::cerr << "stbtt_InitFont() Failed!\n";
            return false;
        }

        const uint32_t fontAtlasWidth = 512;
        const uint32_t fontAtlasHeight = 512;

        textureWidth = fontAtlasWidth;
        textureHeight = fontAtlasHeight;

        codePointOfFirstChar = 32;                     // ASCII of ' '(Space)
        const uint32_t charsToIncludeInFontAtlas = 95; // Include 95 charecters

        std::vector<uint8_t> fontAtlasTextureData;
        std::vector<stbtt_packedchar> packedChars;
        std::vector<stbtt_aligned_quad> alignedQuads;

        fontAtlasTextureData.resize(fontAtlasWidth * fontAtlasHeight);
        packedChars.resize(charsToIncludeInFontAtlas);
        alignedQuads.resize(charsToIncludeInFontAtlas);
        characters.resize(charsToIncludeInFontAtlas);
        quads.resize(charsToIncludeInFontAtlas);

        float fontSize = (float)pixelSize;
        
        stbtt_pack_context ctx;

        stbtt_PackBegin(
            &ctx,                                     // stbtt_pack_context (this call will initialize it) 
            fontAtlasTextureData.data(),              // Font Atlas texture data
            fontAtlasWidth,                           // Width of the font atlas texture
            fontAtlasHeight,                          // Height of the font atlas texture
            0,                                        // Stride in bytes
            1,                                        // Padding between the glyphs
            nullptr);

        stbtt_PackFontRange(
            &ctx,                                     // stbtt_pack_context
            data,                                     // Font Atlas texture data
            0,                                        // Font Index                                 
            fontSize,                                 // Size of font in pixels. (Use STBTT_POINT_SIZE(fontSize) to use points) 
            codePointOfFirstChar,                     // Code point of the first charecter
            charsToIncludeInFontAtlas,                // No. of charecters to be included in the font atlas 
            packedChars.data()                        // stbtt_packedchar array, this struct will contain the data to render a glyph
        );

        stbtt_PackEnd(&ctx);

        for (int i = 0; i < charsToIncludeInFontAtlas; i++) {
            float unusedX, unusedY;

            stbtt_GetPackedQuad(
                packedChars.data(),                  // Array of stbtt_packedchar
                fontAtlasWidth,                      // Width of the font atlas texture
                fontAtlasHeight,                     // Height of the font atlas texture
                i,                                   // Index of the glyph
                &unusedX, &unusedY,                  // current position of the glyph in screen pixel coordinates, (not required as we have a different corrdinate system)
                &alignedQuads[i],                    // stbtt_alligned_quad struct. (this struct mainly consists of the texture coordinates)
                0                                    // Allign X and Y position to a integer (doesn't matter because we are not using 'unusedX' and 'unusedY')
            );
        }

        memcpy(characters.data(), packedChars.data(), packedChars.size() * sizeof(stbtt_packedchar));
        memcpy(quads.data(), alignedQuads.data(), alignedQuads.size() * sizeof(stbtt_aligned_quad));

        lineHeight = 0.0f;

        //Estimate line height based on the maximum height of all glyphs
        for (const auto &glyph : characters) {
            float glyphHeight = (glyph.y1 - glyph.y0);
            if (glyphHeight > lineHeight) {
                lineHeight = glyphHeight;
            }
        }

        // // Scale factors
        // float scale = stbtt_ScaleForPixelHeight(&fontInfo, fontSize);

        // // Get font metrics
        // int ascent, descent, lineGap;
        // stbtt_GetFontVMetrics(&fontInfo, &ascent, &descent, &lineGap);

        // lineHeight = (ascent - descent + lineGap) * scale;

        //Convert to 4 channel texture
        uint8_t *pixels = new uint8_t[fontAtlasTextureData.size() * 4];
        size_t index = 0;

        for(size_t i = 0; i < fontAtlasTextureData.size(); i++) {
            pixels[index+0] = fontAtlasTextureData[i];
            pixels[index+1] = fontAtlasTextureData[i];
            pixels[index+2] = fontAtlasTextureData[i];
            pixels[index+3] = fontAtlasTextureData[i];
            index += 4;
        }

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // The given texture data is a single channel 1 byte per pixel data 
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, fontAtlasWidth, fontAtlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, fontAtlasTextureData.data());
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, fontAtlasWidth, fontAtlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_2D, 0);

        delete[] pixels;

        return true;
    }

    bool Font::add(const Font &font, const std::string &name) {
        if(fonts.count(name) > 0)
            return false;
        fonts[name] = font;
        return true;
    }

    bool Font::remove(const std::string &name) {
        if(fonts.count(name) == 0)
            return false;
        fonts[name].destroy();
        fonts.erase(name);
        return true;
    }

    Font *Font::find(const std::string &name) {
        if(fonts.count(name) > 0)
            return &fonts[name];
        return nullptr;
    }
}