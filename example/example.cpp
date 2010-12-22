#include "OGRE/Ogre.h"
#include "OIS/OIS.h"
#include "Gorilla.h"
#include "Monkey.h"

#pragma warning ( disable : 4244 )

class App : public Ogre::FrameListener, public OIS::KeyListener, public OIS::MouseListener
{
  
 public:
  
  Monkey::PuzzleTree* mTree;
  
  App() : mTree(0)
  {
   
   _makeOgre();
   _makeOIS();
   
   mTree = new Monkey::PuzzleTree("rendezvous.monkey-css", mViewport);
   Monkey::Element* test = mTree->createElement("#test notgreen needabiggerboat", 30, 30, 320, 100);
   test->setText("Hello Monkey!");
   
   Monkey::Element* immonkey = test->createChild("notblue", 1,1, 20,20);
   immonkey->setText("I'm Monkey!");
   
   immonkey->createChild("notblue", 1,1, 10,10);
  }
  
 ~App()
  {
   delete mTree;
   delete mRoot;
  }
  
  bool frameStarted(const Ogre::FrameEvent& evt)
  {
   
   if (mWindow->isClosed())
    return false;
   
   mKeyboard->capture();
   if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
     return false;
   mMouse->capture();
   
   return true;
  }
  
  bool keyPressed( const OIS::KeyEvent &e )
  {
   return true;
  }
  
  bool keyReleased( const OIS::KeyEvent &e )
  {
   return true;
  }
  
  bool mouseMoved( const OIS::MouseEvent &arg )
  {
   return true;
  }
  
  bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
  {
   return true;
  }
  
  bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
  {
   return true;
  }
  
  void _makeOgre()
  {
   srand(time(0));
   
   mRoot = new Ogre::Root("","");
   mRoot->addFrameListener(this);
#if 1
#ifdef _DEBUG
   mRoot->loadPlugin("RenderSystem_Direct3D9_d");
#else
   mRoot->loadPlugin("RenderSystem_Direct3D9");
#endif
#else
#ifdef _DEBUG
   mRoot->loadPlugin("RenderSystem_GL_d.dll");
#else
   mRoot->loadPlugin("RenderSystem_GL.dll");
#endif
#endif
   
   mRoot->setRenderSystem(mRoot->getAvailableRenderers()[0]);
    
   Ogre::ResourceGroupManager* rgm = Ogre::ResourceGroupManager::getSingletonPtr();
   rgm->addResourceLocation(".", "FileSystem");
   
   mRoot->initialise(false);
   
   mWindow = mRoot->createRenderWindow("Monkey", 1024, 768, false);
   mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
   mCamera = mSceneMgr->createCamera("Camera");
   mViewport = mWindow->addViewport(mCamera);
   mViewport->setBackgroundColour(Gorilla::rgb(85,183,40));
   
   rgm->initialiseAllResourceGroups();
  }
  
  void _makeOIS()
  {
   // Initialise OIS
   OIS::ParamList pl;
   size_t windowHnd = 0;
   std::ostringstream windowHndStr;
   mWindow->getCustomAttribute("WINDOW", &windowHnd);
   windowHndStr << windowHnd;
   pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
   mInputManager = OIS::InputManager::createInputSystem( pl );
   mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
   mKeyboard->setEventCallback(this);
   mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
   mMouse->setEventCallback(this);
   mMouse->getMouseState().width = mViewport->getActualWidth();
   mMouse->getMouseState().height = mViewport->getActualHeight();
  }
  
  Ogre::Root* mRoot;
  Ogre::RenderWindow* mWindow;
  Ogre::Viewport* mViewport;
  Ogre::SceneManager* mSceneMgr;
  Ogre::Camera* mCamera;
  Ogre::Real mNextUpdate;
  OIS::InputManager* mInputManager;
  OIS::Keyboard* mKeyboard;
  OIS::Mouse* mMouse;
  
};

void main()
{
 
 try
 {
  App* app = new App();
  app->mRoot->startRendering();
  delete app;
 }
 catch(Ogre::Exception& e)
 {
  std::cout << "--------\n" << e.getFullDescription() << "\n\n";
 }
 
}



