#ifndef ML_MACRO_H_
#define ML_MACRO_H_

#define SINGLETON_INSTANCE(class_name) class_name::GetInstance();

#define SINGLETON_DEFINITION(class_name) class_name::CGarbo Garbo##class_name; \
class_name *class_name::m_pInstance = 0;

//no need define destructior public and should mutex new classname()
#define SINGLETON(class_name) \
private:    \
    virtual ~class_name(); \
public: \
    static class_name* GetInstance() \
    { \
         if(m_pInstance == 0) \
         { \
           m_pInstance = new class_name(); \
         } \
 \
         return m_pInstance; \
    } \
 \
public: \
    class CGarbo \
    { \
      public: \
           ~CGarbo() \
           { \
              if(class_name::m_pInstance) \
              { \
                  delete class_name::m_pInstance; \
              } \
           } \
       \
     }; \
 \
    static CGarbo Garbo; \
 \
private: \
    class_name(); \
    static class_name* m_pInstance;

#define DELETE_NOUSE_TRIVIAL_FUNC(class_name) \
private: \
    class_name(const class_name&) = delete; \
    class_name(class_name&&) = delete; \
    class_name& operator = (const class_name&) = delete; \
    class_name& operator = (class_name&&) = delete;

#endif // ML_MACRO_H_
