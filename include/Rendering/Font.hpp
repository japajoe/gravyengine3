#ifndef GRAVYENGINE_FONT_HPP
#define GRAVYENGINE_FONT_HPP

#include "../System/Drawing/Image.hpp"
#include "../External/stb/stb_truetype.h"
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <unordered_map>

// typedef struct FT_FaceRec_* FT_Face;

// namespace GravyEngine 
// {
//     struct Glyph 
//     {
//         int32_t sizeX, sizeY;
//         int32_t advanceX, advanceY;
//         int32_t bearingX, bearingY;
//         int64_t height;
//         int32_t bottomBearing;
//         int32_t leftBearing;
//         float u0, v0;
//         float u1, v1;
//     };

//     enum class FontMode 
//     {
//         Normal,
//         SDF
//     };

//     class Font 
//     {
//     public:
//         Font();
//         Font(const Font &other);
//         Font(Font &&other) noexcept;
//         Font& operator=(const Font &other);
//         Font& operator=(Font &&other) noexcept;
//         bool LoadFromFile(const std::string &filepath, uint32_t pixelSize, FontMode fontMode = FontMode::SDF, bool createTexture = true);
//         bool LoadFromMemory(const void *data, size_t dataSize, uint32_t pixelSize, FontMode fontMode = FontMode::SDF, bool createTexture = true);
//         bool GenerateTexture();
//         void Destroy();
//         uint32_t GetPixelSize() const;
//         uint32_t GetMaxHeight() const;
//         uint32_t GetTexture() const;
//         Glyph *GetGlyph(char c);
//         uint32_t GetCodePointOfFirstChar() const;
//         void CalculateBounds(const char *text, size_t size, float fontSize, float &width, float &height);
//         void CalculateCharacterPosition(const char *text, size_t size, size_t characterIndex, float fontSize, float &x, float &y);
//         float CalculateYOffset(const char *text, size_t size, float fontSize);
//         float GetPixelScale(float fontSize) const;
//         float GetLineHeight() const;
//         static Font *Add(const std::string &name, const Font &font);
//         static Font *Find(const std::string &name);
//         static void Remove(const std::string &name, const Font &font);
//     private:
//         uint32_t pixelSize;
//         uint32_t textureId;
//         uint32_t maxHeight;
//         uint32_t lineHeight;
//         uint32_t codePointOfFirstChar;
//         std::vector<Glyph> glyphs;
//         std::vector<uint8_t> textureData;
//         static std::unordered_map<std::string,Font> fonts;
//         bool Load(FT_Face fontFace, FontMode fontMode);
//     };
// }

namespace GravyEngine
{
    struct PackedChar
    {
        unsigned short x0, y0, x1, y1; // coordinates of bbox in bitmap
        float xoff, yoff, xadvance;
        float xoff2, yoff2;
    };

    struct AlignedQuad
    {
        float x0, y0, s0, t0; // top-left
        float x1, y1, s1, t1; // bottom-right
    };

    class Font {
    public:
        Font();
        Font(const std::string &filepath, uint32_t pixelSize);
        Font(const uint8_t *fontData, uint32_t pixelSize);
        Font(const Font &other);
        void destroy();
        float computeLineHeight(const std::string &text, float fontSize);
        float computeTextWidth(const std::string &text, float fontSize);
        float computeTextHeight(const std::string &text, float fontSize);
        float getStringWidth(const std::string &text, uint32_t from, uint32_t to, float fontSize);
        int getCodePoint(const std::string &text, int to, int i, int32_t &cpOut);
        void computeCursorPosition(const std::string &text, size_t cursorIndex, float fontSize, float &x, float &y);
        inline uint32_t getPixelSize() const { return pixelSize; }
        inline uint32_t getTexture() const { return texture; }
        inline uint32_t getTextureWidth() const { return textureWidth; }
        inline uint32_t getTextureHeight() const { return textureHeight; }
        inline std::vector<PackedChar> &getCharacters() { return characters; }
        inline std::vector<AlignedQuad> &getQuads() { return quads; }
        inline uint32_t getCodePointOfFirstChar() { return codePointOfFirstChar; }
        inline float getLineHeight() const { return lineHeight; }
        inline size_t getNumberOfCharacters() const { return characters.size(); }
        static bool add(const Font &font, const std::string &name);
        static bool remove(const std::string &name);
        static Font *find(const std::string &name);
    private:
        stbtt_fontinfo fontInfo;
        uint32_t pixelSize;
        uint32_t texture;
        uint32_t codePointOfFirstChar;
        uint32_t textureWidth;
        uint32_t textureHeight;
        float lineHeight;
        std::vector<PackedChar> characters;
        std::vector<AlignedQuad> quads;
        static std::unordered_map<std::string, Font> fonts;
        bool loadFromFile(const std::string &filepath);
        bool loadFromMemory(const uint8_t *fontData);
        bool load(const uint8_t *data);
        static bool isHighSurrogate(char32_t c);
        static bool isLowSurrogate(char32_t c);
    };
}

#endif