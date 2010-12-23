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
#include "OIS/OIS.h"
#include "Gorilla.h"


namespace Monkey
{
 
 enum Unit
 {
  Unit_Pixel,
  Unit_Percent,
  Unit_AlignRight,
  Unit_AlignCenter
 };

 class Element;
 struct ElementStyle;
 class PuzzleTree;
 
 class Callback
 {
  public:
   // Mouse not moving, mouse clicked once.
   virtual void onClick(Monkey::Element*, const OIS::MouseState&) {}
   // Mouse not moving, mouse clicked twice quickly.
   virtual void onDoubleClick(Monkey::Element*, const OIS::MouseState&) {}
   // Mouse moving.
   virtual void onMouseMove(Monkey::Element*, const OIS::MouseState&) {}
   // Mouse moving, with any button held down.
   virtual void onMouseDrag(Monkey::Element*, const OIS::MouseState&) {}
 };
 
 class PuzzleTree 
 {
   
  public:
   
   friend class Element;
   
   // PuzzleTree constructor. 
   // Note: If Gorilla's Silverback hasn't been created, PuzzleTree will create it.
   PuzzleTree(const Ogre::String& monkey_css, Ogre::Viewport*, Callback* callback);
   
  ~PuzzleTree();
   
   Element* createElement(const Ogre::String& css_id_or_classes);
   
   Callback* getCallback() const { return mCallback; }
   
   void maml(const Ogre::String& maml_string);
   
   void loadCSS(const Ogre::String& monkey_css);
   
   void mouseMoved( const OIS::MouseEvent &arg );
   
   void mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
   
   void mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
   
   Gorilla::Silverback*  getSilverback() const { return mSilverback; }

   Gorilla::Screen* getScreen() const { return mScreen; }

   void dumpCSS();

   ElementStyle* getStyle(const Ogre::String& name)
   {
    std::map<Ogre::String, ElementStyle*>::iterator it = mStyles.find(name);
    if (it == mStyles.end())
     return 0;
    return (*it).second;
   }
   
   
  protected:
   
   std::vector<Element*>                      mMouseListenerElements;
   std::multimap<Ogre::String, Element*>      mElements;
   std::map<Ogre::String, ElementStyle*>      mStyles;
   Gorilla::Silverback*                       mSilverback;
   Gorilla::Screen*                           mScreen;
   Ogre::Viewport*                            mViewport;
   Gorilla::Layer*                            mLayers[16];
   Ogre::String                               mAtlas;
   OIS::Mouse*                                mMouse;
   Element*                                   mMousePointer;
   Callback*                                  mCallback;
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
   float width;
   Unit width_unit;
   bool width_set;
   float height;
   Unit height_unit;
   bool height_set;
   float left;
   Unit left_unit;
   bool left_set;
   int top;
   Unit top_unit;
   bool top_set;
   void reset();
   void to_css(Ogre::String&);
   void from_css(const Ogre::String& key, const Ogre::String& value);
   void merge(ElementStyle&, bool isParent);
  };

  class Element
  {
    
   public:
    
    Element(const std::string& id_and_or_classes, Gorilla::Layer*, PuzzleTree*, Element*, size_t index);
    
   ~Element();
    
    Element* createChild(const std::string& id_and_or_classes);
    
    void listen()
    {
     mTree->mMouseListenerElements.push_back(this);
    }
    
    void unlisten()
    {
     mTree->mMouseListenerElements.erase(std::find(mTree->mMouseListenerElements.begin(), mTree->mMouseListenerElements.end(), this));
    }
    
    bool onClick(const Ogre::Vector2& coords, const OIS::MouseEvent&);
    bool onDoubleClick(const Ogre::Vector2& coords, const OIS::MouseEvent&);
    bool onMouseMove(const Ogre::Vector2& coords, const OIS::MouseEvent&);
    bool onMouseDrag(const Ogre::Vector2& coords, const OIS::MouseEvent&);

    void setText(const Ogre::String& text)   { mText = text; reapplyLook(); }
    
    Ogre::String getID() const { return mID; }
    
    float getScreenLeft() const
    {
     if (mRectangle)
      return mRectangle->left();
     else if (mCaption)
      return mCaption->left();
     return 0;
    }
    
    float getScreenTop() const
    {
     if (mRectangle)
      return mRectangle->top();
     else if (mCaption)
      return mCaption->top();
     return 0;
    }

    float getScreenWidth() const
    {
     if (mRectangle)
      return mRectangle->width();
     else if (mCaption)
      return mCaption->width();
     return 0;
    }
    
    float getScreenHeight() const
    {
     if (mRectangle)
      return mRectangle->height();
     else if (mCaption)
      return mCaption->width();
     return 0;
    }
    
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
  };
  
}

#endif
