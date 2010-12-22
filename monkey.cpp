/*
    Monkey
    ------
    
    Copyright (c) 2010 Robin Southern
                                                                                  
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
                                                                                  
    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
                                                                                  
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE. 
    
*/

#include "Monkey.h"

#pragma warning ( disable : 4244 )

namespace Monkey
{

namespace SecretMonkey
{
 
 typedef std::string String;
 typedef std::pair<std::string, std::string> StringPair;
 
 
static const String bool_true = "true";
static const String bool_false = "false";
static const String whitespace = " \t\r\n";
static const String newlines = "\r\n";

 size_t index(const String& string, char search)
 {
  return string.find_first_of(search);
 }

 bool has(const String& string, char search)
 {
  return string.find_first_of(search) != String::npos;
 }

 void trim(String& string)
 {
  string.erase(string.find_last_not_of(whitespace)+1);
  string.erase(0, string.find_first_not_of(whitespace));
 }

 String trim_copy(const String& string)
 {
  String dest(string);
  trim(dest);
  return dest;
 }

 void lower(String& string)
 {
  std::transform(string.begin(), string.end(), string.begin(), tolower);
 }

 String lower_copy(const String& original)
 {
  String dest;
  dest.append(original);
  lower(dest);
  return dest;
 }
 
 void upper(String& string)
 {
  std::transform(string.begin(), string.end(), string.begin(), toupper);
 }

 String upper_copy(const String& original)
 {
  String dest;
  dest.append(original);
  upper(dest);
  return dest;
 }
 
 void captialise(String& string)
 {
  lower(string);
  if (string.length())
   toupper(string[0]);
 }

 String captialise_copy(const String& string)
 {
  String dest(string);
  captialise(dest);
  return dest;
 }

 void slice_to_first_of(String& string, char pattern)
 {
  size_t pos = string.find_first_of(pattern);
  if (pos != String::npos)
   string.assign(string.substr(0, pos));
 }
 
 void slice_to_first_of(String& string, String& pattern)
 {
  size_t pos = string.find_first_of(pattern);
  if (pos != String::npos)
   string.assign(string.substr(0, pos));
 }

 void slice_to_last_of(String& string, char pattern)
 {
  size_t pos = string.find_last_of(pattern);
  if (pos != String::npos)
   string.assign(string.substr(0, pos));
 }

 void slice_after_first_of(String& string, char pattern)
 {
  size_t pos = string.find_first_of(pattern);
  if (pos != String::npos)
   string.assign(string.substr(pos+1));
 }

 void slice_after_last_of(String& string, char pattern)
 {
  size_t pos = string.find_last_of(pattern);
  if (pos != String::npos)
   string.assign(string.substr(pos+1));
 }


 void slice(String& string, size_t begin)
 {
  string.assign(string.substr(begin));
 }

 void slice(String& string, size_t begin, size_t end)
 {
  string.assign(string.substr(begin, end));
 }

 String slice_copy(const String& string, size_t begin)
 {
  return string.substr(begin);
 }

 String slice_copy(const String& string, size_t begin, size_t end)
 {
  return string.substr(begin, end);
 }

 bool starts(const String& original, const String& comparision)
 {
  return original.compare(0, comparision.length(), comparision) == 0;
 }

 bool starts_insensitive(const String& original, const String& comparision)
 {
  if (original.length() < comparision.length())
   return false;
 
  for (size_t it = 0; it < comparision.length();++it)
   if (tolower(original[it]) != tolower(comparision[it]))
    return false;
 
  return true;
 }

 bool matches(const String& original, const String& comparision)
 {
  return original == comparision;
 }

 bool matches_insensitive(const String& original, const String& comparision)
 {
  return lower_copy(original) == lower_copy(comparision);
 }
 
 StringPair cut(const String& str, bool& didCut, char delimiter, char endDelimiter)
 {
 
  didCut = false;
 
  String working(str);
 
  if (endDelimiter != 0)
   slice_to_first_of(working, endDelimiter);
 
  trim(working);
 
  if (working.length() == 0)
   return StringPair(String(), String());
 
  if (index(working, delimiter) == std::string::npos)
   return StringPair(String(), String());

  std::string first(working);
  slice_to_first_of(first, delimiter);
  std::string second(working);
  slice_after_first_of(second, delimiter);
 
  trim(first);
  trim(second);
 
  didCut = true;
 
  return StringPair(first, second);
 }
 
 std::vector<std::string> split(const String& str, const String& delimiters);

 std::map<std::string, std::string> cut_many(const String& str, char delimiter, char endDelimiter)
{
 std::map<std::string, std::string> ret;
 
 if (str.length() <= 1)
  return ret;

 std::vector<std::string> lines = split(str, "\n");
 
 for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); it++)
 {
  String working(str);
  
  if (endDelimiter != 0)
   slice_to_first_of(working, endDelimiter);
  
  trim(working);
  
  if (working.length() == 0)
   continue;
  
  if (index(working, delimiter) == std::string::npos)
   continue;
  
  std::string first(working);
  slice_to_first_of(first, delimiter);
  std::string second(working);
  slice_after_first_of(second, delimiter);
  
  trim(first);
  trim(second);
  
  ret.insert(std::pair<std::string, std::string>(first, second));
 }
 
 return ret;
}

std::vector<std::string> split(const String& str, const String& delimiters)
{
 std::vector<std::string> out;
 size_t b = str.find_first_not_of(delimiters, 0);
 size_t a = str.find_first_of(delimiters, b);
 std::string t;
 while (a != String::npos || b != String::npos)
 {
  std::stringstream s(str.substr(b, a - b));
  s >> t;
  out.push_back(t);
  b = str.find_first_not_of(delimiters, a);
  a = str.find_first_of(delimiters, b);
 }

 return out;
}

bool isInteger(const std::string& str)
{
 bool hasExponent = false;
 for (std::string::const_iterator it = str.begin(); it != str.end(); it++)
 {
  
  // Sign -- Must be the first character of the string.
  if ((*it) == '-' || (*it) == '+')
  {
   if (it == str.begin())
    continue;
   
   return false;
  }
  
  // Exponent -- Can't be at the first or last part of the string, and there can only be one of them.
  if ((*it) == 'E' || (*it) == 'e')
  {
   if (hasExponent || it == str.begin() || it == str.end())
    return false;
   hasExponent = true;
   continue;
  }
  
  if (isdigit((*it)) == false)
   return false;
  
 }
 return true;
}

bool isReal(const std::string& str)
{
 bool hasDecimal = false, hasExponent = false, hasSign = false;
 for (std::string::const_iterator it = str.begin(); it != str.end(); it++)
 {
  
  // Sign -- Must be the first character of the string, or if the previous character was an exponent.
  if ((*it) == '-' || (*it) == '+')
  {
   
   hasSign = true;
   
   if (it == str.begin())
    continue;

   std::string::const_iterator last = it - 1;
   if ( (*last) == 'E' || (*last) == 'e')
    continue;
   
   return false;
  }
  
  // Exponent -- Can't be at the first or last part of the string, and there can only be one of them.
  if ((*it) == 'E' || (*it) == 'e')
  {
   if (hasExponent || hasDecimal || hasSign || it == str.begin() || it == str.end())
    return false;
   hasExponent = true;
   continue;
  }
  
  // Decimal -- There can only be one of them. There can't be one if there is an exponent.
  if ((*it) == '.')
  {
   if (hasExponent || hasDecimal)
    return false;
   hasDecimal = true;
   continue;
  }

  if (isdigit((*it)) == false)
   return false;
 }
 
 return true;
}

bool isBoolean(const std::string& str)
{
 std::string working(str);
 std::transform(working.begin(), working.end(), working.begin(), tolower);
 return working == "true" || working == "false";
}

bool split(const String& str, std::map<String, String>& value, char delimiter, bool lowerKey, bool trimString)
{
 size_t pos = str.find_first_of(delimiter);
 if (pos == String::npos)
  return false;
 
 std::string k = str.substr(0,pos);
 std::string v = str.substr(pos+1);

 if (lowerKey)
  lower(k);

 if (trimString)
 {
  trim(k);
  trim(v);
 }
 
 value[k] = v;

 return true;
}

Ogre::ColourValue getCSSRGBAColour(const Ogre::String& value)
{
 
 Ogre::String working(value);
 slice_after_first_of(working, '(');
 slice_to_last_of(working, ')');

 Ogre::vector<Ogre::String>::type strs = Ogre::StringUtil::split(working, ",", 4);
 // split into four.
 Ogre::ColourValue colour = Ogre::ColourValue::White;
  
 if (strs.size() >= 3)
 {
  trim(strs[0]);
  colour.r = float(Ogre::StringConverter::parseInt(strs[0])) * (1.0f / 255.0f);
  
  trim(strs[1]);
  colour.g = float(Ogre::StringConverter::parseInt(strs[1])) * (1.0f / 255.0f);
  
  trim(strs[2]);
  colour.b = float(Ogre::StringConverter::parseInt(strs[2])) * (1.0f / 255.0f);
  
  colour.a = 1.0f;
 }
 if (strs.size() == 4)
 {
  trim(strs[3]);
  colour.a = float(Ogre::StringConverter::parseInt(strs[3])) * (1.0f / 255.0f);
 }
 
 return colour;
}

Ogre::String toCSSRGBAColour(const Ogre::ColourValue& colour)
{
 std::stringstream s;
 s << "RGBA(" << int(colour.r * 255.0f) << ", " << int(colour.g * 255.0f) << ", " << int(colour.b * 255.0f) << ", " << int(colour.a * 255.0f) << ")";
 return s.str();
}

} // namespace SecretMonkey

PuzzleTree::PuzzleTree(const Ogre::String& css, Ogre::Viewport* viewport)
: mViewport(viewport)
{
 
 loadCSS(css);
 
 mSilverback = Gorilla::Silverback::getSingletonPtr();
 
 if (mSilverback == 0)
  mSilverback = new Gorilla::Silverback();
 
 mSilverback->loadAtlas(mAtlas);
 mScreen = mSilverback->createScreen(mViewport, mAtlas);

 for (size_t i=0;i < 16;i++)
  mLayers[i] = mScreen->createLayer(i);
 
}

PuzzleTree::~PuzzleTree()
{
 
}

void PuzzleTree::loadCSS(const Ogre::String& css_file_name_path)
{
 namespace S = ::Monkey::SecretMonkey;

 S::StringPair sp;
 bool didCut = false;
 sp = S::cut(css_file_name_path, didCut, ':', 0);

 Ogre::DataStreamPtr stream;
 if (didCut)
  stream = Ogre::ResourceGroupManager::getSingleton().openResource(sp.second, sp.first);
 else
  stream = Ogre::ResourceGroupManager::getSingleton().openResource(css_file_name_path, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
 
 Ogre::String line, element_name;
 Ogre::vector<Ogre::String>::type workings;
 bool inElement = false;
 ElementStyle* style = 0;
 while (!stream->eof())
 {
  line = stream->getLine();
  S::trim(line);
  if (line.length() == 0)
   continue;
  if (S::starts(line, "//"))
   continue;
  S::slice_to_first_of(line, Ogre::String("//"));
  
  if (inElement == false && S::starts(line, "@import"))
  {
   S::slice_after_first_of(line, '"');
   S::slice_after_first_of(line, '\'');
   S::slice_to_last_of(line, '"');
   S::slice_to_last_of(line, '\'');
   S::trim(line);
   mAtlas = line;
   element_name.clear();
   continue;
  }

  if (inElement == false)
  {
   size_t bracket = S::index(line, '{');
   if (bracket != std::string::npos)
   {
    std::string t = line;
    S::slice_to_first_of(t, '{');
    element_name.append(t);
    S::trim(element_name);
    S::slice_after_first_of(line, '{');
    style = new ElementStyle();
    mStyles[element_name] = style;
    style->reset();
    inElement = true;
   }
   else
   {
    element_name = line;
    continue;
   }
  }

  // In Element

  std::string working = line;
  S::slice_to_first_of(working, '}');

  // Parse CSS from working here.
  S::trim(working);

  workings = Ogre::StringUtil::split(working, ";");

  for (size_t i=0;i < workings.size();i++)
  {
   S::trim(workings[i]);
   if (workings[i].length() == 0)
    continue;
   
   sp = S::cut(workings[i], didCut, ':', 0);
   S::lower(sp.first);
   style->from_css(sp.first, sp.second);
  }
  
  if (S::index(line, '}') != std::string::npos)
  {
   inElement = false;
   element_name.clear();
  }

 }

}

void PuzzleTree::dumpCSS()
{
 for (std::map<Ogre::String, ElementStyle*>::iterator it = mStyles.begin(); it != mStyles.end(); it++)
 {
  Ogre::String css;
  (*it).second->to_css(css);
  std::cout << (*it).first << "\n{\n" << css << "}\n";
 }
}


Element* PuzzleTree::createElement(const Ogre::String& css_id_or_classes, unsigned int top, unsigned int left, unsigned int width, unsigned int height)
{
 Element* elem = new Element(css_id_or_classes, top, left, width, height, mLayers[0], this, 0, 0);
 mElements.insert(std::pair<Ogre::String, Element*>(elem->getID(), elem));
 return elem;
}

// ----------------------------------------------------------------------------------------------------------------

void ElementStyle::reset()
{
 alignment.horz = Gorilla::TextAlign_Left;
 alignment.horz_set = false;
 alignment.vert = Gorilla::VerticalAlign_Top;
 alignment.vert_set = false;
 background.colour = Ogre::ColourValue::White;
 background.set = false;
 background.sprite.clear();
 background.type = Background::BT_Transparent;
 colour = Ogre::ColourValue::White;
 colour_set = false;
 font = 9;
 font_set = false;
 border.width = 0;
 border.width_set = false;
 border.top = Ogre::ColourValue::White;
 border.top_set = false;
 border.right= Ogre::ColourValue::White;
 border.right_set = false;
 border.bottom = Ogre::ColourValue::White;
 border.bottom_set = false;
 border.left = Ogre::ColourValue::White;
 border.left_set = false;
}

void ElementStyle::to_css(Ogre::String& css)
{
 namespace S = ::Monkey::SecretMonkey;
 std::stringstream s;
 
 s << "text-align: ";
 if (alignment.horz == Gorilla::TextAlign_Right)
  s << " right;\n";
 else if (alignment.horz == Gorilla::TextAlign_Centre)
  s << " center;\n";
 
 s << "vertical-align: ";
 if (alignment.vert == Gorilla::VerticalAlign_Top)
  s << " top;\n";
 else if (alignment.vert == Gorilla::VerticalAlign_Bottom)
  s << " bottom;\n";
 else if (alignment.vert == Gorilla::VerticalAlign_Middle)
  s << " middle;\n";
 
 
  s << "border-width: " << border.width << ";\n";
  s << "border-top: " << S::toCSSRGBAColour(border.top)  << ";\n";
  s << "border-right: " << S::toCSSRGBAColour(border.right)  << ";\n";
  s << "border-bottom: " << S::toCSSRGBAColour(border.bottom)  << ";\n";
  s << "border-left: " << S::toCSSRGBAColour(border.left)  << ";\n";
 

 if (background.type == Background::BT_Transparent)
  s << "background: none;\n";
 else if (background.type == Background::BT_Sprite)
  s << "background-image: " << background.sprite << ";\n";
 else if (background.type == Background::BT_Colour)
  s << "background-colour: " << S::toCSSRGBAColour(background.colour) << ";\n";
 
 s << "colour: " << S::toCSSRGBAColour(colour) << ";\n";
 
 s << "font: " << font << ";\n";

 css.assign(s.str());
}

void ElementStyle::from_css(const Ogre::String& key, const Ogre::String& value)
{
 
 namespace S = ::Monkey::SecretMonkey;
 Ogre::String working(value);
 S::trim(working);

 if (key == "text-align")
 {
  if (S::matches_insensitive(working, "left"))
   alignment.horz = Gorilla::TextAlign_Left;
  else if (S::matches_insensitive(working, "center"))
   alignment.horz = Gorilla::TextAlign_Centre;
  else if (S::matches_insensitive(working, "centre"))
   alignment.horz = Gorilla::TextAlign_Centre;
  else if (S::matches_insensitive(working, "right"))
   alignment.horz = Gorilla::TextAlign_Right;
  alignment.horz_set = true;
 }
 else if (key == "vertical-align")
 {
  if (S::matches_insensitive(working, "top"))
   alignment.vert = Gorilla::VerticalAlign_Top;
  else if (S::matches_insensitive(working, "middle"))
   alignment.vert = Gorilla::VerticalAlign_Middle;
  else if (S::matches_insensitive(working, "bottom"))
   alignment.vert = Gorilla::VerticalAlign_Bottom;
  alignment.vert_set = true;
 }
 else if (key == "font")
 {
  font = Ogre::StringConverter::parseInt(working);
  font_set = true;
 }
 else if (key == "border")
 {
   // <size> <all-colours>
   Ogre::vector<Ogre::String>::type strs = Ogre::StringUtil::split(working, " ", 1);
   
   if (strs.size() == 1)
   {
    border.width = Ogre::StringConverter::parseInt(strs[0]);
    border.width_set = true;
   }
   else if (strs.size() == 2)
   {
    border.width = Ogre::StringConverter::parseInt(strs[0]);
    border.width_set = true;
    Ogre::ColourValue col;
    border.top = border.left = border.bottom = border.right = S::getCSSRGBAColour(strs[1]);
    border.top_set = border.left_set = border.bottom_set = border.right_set = true;
   }
  
 }
 else if (key == "border-width")
 {
  border.width = Ogre::StringConverter::parseInt(working);
  border.width_set = true;
 }
 else if (key == "border-top")
 {
  border.top = S::getCSSRGBAColour(working);
  border.top_set = true;
 }
 else if (key == "border-right")
 {
  border.right = S::getCSSRGBAColour(working);
  border.right_set = true;
 }
 else if (key == "border-bottom")
 {
  border.bottom = S::getCSSRGBAColour(working);
  border.bottom_set = true;
 }
 else if (key == "border-left")
 {
  border.left = S::getCSSRGBAColour(working);
  border.left_set = true;
 }
 else if (key == "background")
 {
  if (S::matches_insensitive(working, "none"))
   background.type = Background::BT_Transparent;
  else if (S::matches_insensitive(working, "transparent"))
   background.type = Background::BT_Transparent;
  background.set = true;
 }
 else if (key == "background-image")
 {
  background.type = Background::BT_Sprite;
  background.sprite = working;
  background.set = true;
 }
 else if (key == "background-colour" || key == "background-color")
 {
  background.type = Background::BT_Colour;
  if (S::starts_insensitive(working, "rgb"))
  {
   background.colour = S::getCSSRGBAColour(working);
  background.set = true;
  }
 }

 else if (key == "colour" || key == "color")
 {
  if (S::starts_insensitive(working, "rgb"))
  {
   colour = S::getCSSRGBAColour(working);
   colour_set = true;
  }
 }
}

void ElementStyle::merge(ElementStyle& other)
{
 if (alignment.horz_set)
 {
  other.alignment.horz = alignment.horz;
  other.alignment.horz_set = true;
 }
 if (alignment.vert_set)
 {
  other.alignment.vert = alignment.vert;
  other.alignment.vert_set = true;
 }
 
 if (background.set)
 {
  other.background.type = background.type;
  other.background.colour = background.colour;
  other.background.sprite = background.sprite;
  other.background.set = true;
 }
 
 if (border.width_set)
 {
  other.border.width = border.width;
  other.border.width_set = true;
 }
  
 if (border.top_set)
 {
  other.border.top = border.top;
  other.border.top_set = true;
 }
 
 if (border.left_set)
 {
  other.border.left = border.left;
  other.border.left_set = true;
 }
 
 if (border.right_set)
 {
  other.border.right = border.right;
  other.border.right_set = true;
 }
 
 if (border.bottom_set)
 {
  other.border.bottom = border.bottom;
  other.border.bottom_set = true;
 }
 
 if (colour_set)
 {
  other.colour = colour;
  other.colour_set = true;
 }
 
 if (font_set)
 {
  other.font = font;
  other.font_set = true;
 }
 
}


// ----------------------------------------------------------------------------------------------------------------


Element::Element(const std::string& id_and_or_classes, unsigned int x, unsigned int y, unsigned int w, unsigned int h, Gorilla::Layer* layer, PuzzleTree* tree, Element* parent, size_t index)
: mTree(tree), mParent(parent), mLayer(layer), mRectangle(0), mCaption(0), mX(x), mY(y), mWidth(w), mHeight(h), mIndex(index), mHasLimits(false)
{
 
 mLook.reset();
 namespace S = ::Monkey::SecretMonkey;
 
 std::string classes;
 
 classes.append(id_and_or_classes);

 if (mParent != 0)
 {
  // Merge parent's initial style.
  mParent->getStyle()->merge(mLook);
  // Allow for a child's style based on parent-child order...thing.
  classes.append(" #" + mParent->getID() + ":child");   // '#parent:child'
 }
 
 Ogre::vector<Ogre::String>::type strs = Ogre::StringUtil::split(classes, " ");
 
 std::cout << "---\n";
 Ogre::String name;
 for (size_t i=0;i < strs.size();i++)
 {
  std::cout << strs[i] << "\n";
  if (strs[i][0] == '#')
  {
   name = strs[i];
   mID = S::slice_copy(strs[i], 1);
  }
  else
  {
   name.clear();
   name.assign("." + strs[i]);
  }
   
  ElementStyle* style = mTree->getStyle(name);
  if (style == 0)
  {
   std::cout << "[Monkey] Style '" << strs[i] << "' not found!\n";
   continue;
  }
  style->merge(mLook);
 }
 

 reapplyLook();
 
}

Element::~Element()
{
 // TODO
}

Element* Element::createChild(const std::string& id_and_or_classes, unsigned int x, unsigned int y, unsigned int w, unsigned int h)
{
 size_t index = mIndex + 1;
 if (index >= 14)
  index = 14;
 Element* elem = new Element(id_and_or_classes, x, y, w, h, mTree->mLayers[index], mTree, this, index);
 mTree->mElements.insert(std::pair<Ogre::String, Element*>(elem->getID(), elem));
 mChildren.insert(std::pair<Ogre::String, Element*>(elem->getID(), elem));
 return elem;
}

void Element::destroyChild(Element*)
{
}

void Element::reapplyLook()
{
 
 int x = mX ,y = mY, w = mWidth, h = mHeight;
 bool widthReduced = false, heightReduced = false;
 
 if (mParent)
 {
  x += mParent->getLeft();
  y += mParent->getTop();
  
  if (mParent->hasLimits())
  {
   if (x > mParent->getLeft() + mParent->getWidth() || y > mParent->getTop() + mParent->getHeight())
   {
    if (mRectangle)
    {
     mLayer->destroyRectangle(mRectangle);
     mRectangle = 0;
    }
    if (mCaption)
    {
     mLayer->destroyCaption(mCaption);
     mCaption = 0;
    }
    return;
   }
   
   int maxW = mParent->getWidth() - mX,
       maxH = mParent->getHeight() - mY;
   if (w > maxW)
   {
    w = maxW;
    widthReduced = true;
   }
   if (h > maxH)
   {
    h = maxH;
    heightReduced = true;
   }
  } // if limits
 } // if parent
  
 if (mText.length() != 0)
 {
  if (mCaption == 0)
   mCaption = mLayer->createCaption(mLook.font, x, y, mText);
  mCaption->colour(mLook.colour);
  
  if (mLook.background.type != ElementStyle::Background::BT_Transparent || mLook.border.width != 0)
  {
   mCaption->align(mLook.alignment.horz);
   mCaption->vertical_align(mLook.alignment.vert);
  }
  else
  {
   mCaption->align(Gorilla::TextAlign_Left);
   mCaption->vertical_align(Gorilla::VerticalAlign_Top);
  }
  mCaption->width(w);
  mCaption->height(h);
  mCaption->left(x);
  mCaption->top(y);
  mCaption->colour(mLook.colour);
  mCaption->no_background();
  mCaption->font(mLook.font);
  mCaption->text(mText);
 }

 if (mText.length() == 0 && mCaption != 0)
 {
  mLayer->destroyCaption(mCaption);
  mCaption = 0;
 }
 
 if (mLook.background.type != ElementStyle::Background::BT_Transparent || mLook.border.width != 0)
 {
  mHasLimits = true;
  if (mRectangle == 0)
   mRectangle = mLayer->createRectangle(x, y, w, h);
  
  if (mLook.background.type == ElementStyle::Background::BT_Colour)
   mRectangle->background_colour(mLook.background.colour);
  else if (mLook.background.type == ElementStyle::Background::BT_Sprite)
  {
   if (widthReduced || heightReduced)
   {
    Ogre::Real sx = float(w) / float(mWidth);
    Ogre::Real sy = float(h) / float(mHeight);
    mRectangle->background_image(mLook.background.sprite, sx, sy);
   }
   else
   {
    mRectangle->background_image(mLook.background.sprite);
   }
  }
  else
   mRectangle->no_background();
  
  if (mLook.border.width == 0)
   mRectangle->no_border();
  else
   mRectangle->border(mLook.border.width, mLook.border.top, mLook.border.right, mLook.border.bottom, mLook.border.left);
 
  mRectangle->position(x, y);
  mRectangle->width(w);
  mRectangle->height(h);
 }
 else
 {
  mHasLimits = false;
  if (mRectangle)
  {
   mLayer->destroyRectangle(mRectangle);
   mRectangle = 0;
  }
 }
 
 std::cout << "Stats: " << mID << "\n x,y = " << x << ", " << y << "\n w,h = " << w << "," << h << "\n Rectangle=" << mRectangle << "\n Caption=" << mCaption << "\n";

}

} // namespace Monkey
