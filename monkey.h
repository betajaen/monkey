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

#ifndef MONKEY_H
#define MONKEY_H

#include "OGRE/Ogre.h"
#include "Gorilla.h"


namespace Monkey
{
 
 class Element;
 class PuzzleTree;

 class PuzzleTree
 {
   
  public:
   
   PuzzleTree(const std::string& monkey_css);
   
   PuzzleTree(const std::string& monkey_css, Gorilla::Silverback*);
   
  ~PuzzleTree();
   
   void createElement(const std::string& css_id_or_classes, unsigned int top, unsigned int left);
   
   void createElement(const std::string& css_id_or_classes, unsigned int top, unsigned int left, unsigned int implicitWidth, unsigned int implicitHeight);
   
   void destroyElement(Element*);
   
   void destroyElement(const std::string& css_id);
   
 };
 
  struct ElementProperty
  {
   struct Background
   {
    enum BackgroundType { BT_Transparent, BT_Colour, BT_Sprite };
    BackgroundType type;
    Ogre::ColourValue colour;
    Gorilla::Sprite*  sprite;
   } background;
   Ogre::ColourValue colour;
   size_t font;
   struct TextAligment
   {
    Gorilla::TextAlignment horz;
    Gorilla::VerticalAlignment vert;
   } alignment;
   struct Border
   {
    size_t width;
    Ogre::ColourValue top, left, right, bottom;
   } border;
   float opacity;
   void to_css(Ogre::String&)
   void from_css(const Ogre::String&);
   void merge(ElementProperty&);
  };

  class Element
  {
   ElementProperty mLook;
  };
}

#endif
