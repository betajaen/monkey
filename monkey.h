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
 struct ElementStyle;
 class PuzzleTree;

 class PuzzleTree
 {
   
  public:
   
   friend class Element;
   
   // PuzzleTree constructor. 
   // Note: If Gorilla's Silverback hasn't been created, PuzzleTree will create it.
   PuzzleTree(const Ogre::String& monkey_css, Ogre::Viewport*);
   
      PuzzleTree() {} // temp.
   

  ~PuzzleTree();
   
   Element* createElement(const Ogre::String& css_id_or_classes, unsigned int top, unsigned int left, unsigned int width, unsigned int height);
   
   void destroyElement(Element*);
   
   void destroyElement(const Ogre::String& css_id);
   
   void loadCSS(const Ogre::String& monkey_css);
   
   Gorilla::Silverback*  getSilverback() const { return mSilverback; }

   void dumpCSS();

   ElementStyle* getStyle(const Ogre::String& name)
   {
    std::map<Ogre::String, ElementStyle*>::iterator it = mStyles.find(name);
    if (it == mStyles.end())
     return 0;
    return (*it).second;
   }
   
   
  protected:
   
   std::multimap<Ogre::String, Element*>      mElements;
   std::map<Ogre::String, ElementStyle*>      mStyles;
   Gorilla::Silverback*                       mSilverback;
   Gorilla::Screen*                           mScreen;
   Ogre::Viewport*                            mViewport;
   Gorilla::Layer*                            mLayers[16];
   Ogre::String                               mAtlas;
 };
 
  struct ElementStyle
  {
   struct Background
   {
    enum BackgroundType { BT_Transparent, BT_Colour, BT_Sprite };
    BackgroundType type;
    Ogre::ColourValue colour;
    Ogre::String sprite;
    bool set;
   } background;
   Ogre::ColourValue colour;
   bool colour_set;
   size_t font;
   bool font_set;
   struct TextAligment
   {
    Gorilla::TextAlignment horz;
    Gorilla::VerticalAlignment vert;
    bool horz_set;
    bool vert_set;
   } alignment;
   struct Border
   {
    size_t width;
    bool width_set;
    Ogre::ColourValue top, left, right, bottom;
    bool top_set, left_set, right_set, bottom_set;
   } border;
   void reset();
   void to_css(Ogre::String&);
   void from_css(const Ogre::String& key, const Ogre::String& value);
   void merge(ElementStyle&);
  };

  class Element
  {
    
   public:
    
    Element(const std::string& id_and_or_classes, unsigned int x, unsigned int y, unsigned int w, unsigned int h, Gorilla::Layer*, PuzzleTree*, Element*, size_t index);
    
   ~Element();
    
    Element* createChild(const std::string& id_and_or_classes, unsigned int x, unsigned int y, unsigned int w = 0, unsigned int h = 0);
    void destroyChild(Element*);

    void setTop(unsigned int top)   { mY = top; reapplyLook(); }
    void setLeft(unsigned int left)   { mX = left; reapplyLook(); }
    void setWidth(unsigned int width)   { mWidth = width; reapplyLook(); }
    void setHeight(unsigned int height)   { mHeight = height; reapplyLook(); }
    void setText(const Ogre::String& text)   { mText = text; reapplyLook(); }
    
    Ogre::String getID() const { return mID; }
    unsigned int getTop() const
    {
     if (mParent)
      return mParent->getTop() + mY;
     else
      return mY;
    }
    unsigned int getLeft() const
    {
     if (mParent)
      return mParent->getLeft() + mX;
     else
      return mX;
    }
    bool hasLimits() const { return mHasLimits; }
    unsigned int getWidth() const { return mWidth; }
    unsigned int getHeight() const { return mHeight; }
    Ogre::String getText() const { return mText; }
    
    ElementStyle* getStyle() { return &mLook; }
    
    void reapplyLook();
    
   protected:
    
    PuzzleTree*               mTree;
    Element*                  mParent;
    std::multimap<Ogre::String, Element*>      mChildren;
    Ogre::String              mID;
    std::vector<Ogre::String> mStyles;
    ElementStyle              mLook;
    Gorilla::Caption*         mCaption;
    Gorilla::Rectangle*       mRectangle;
    Ogre::String              mText;
    Gorilla::Layer*           mLayer;
    size_t                    mIndex;
    unsigned int              mX, mY, mWidth, mHeight;
    bool                      mHasLimits;
  };
  
}

#endif
