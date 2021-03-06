#pragma once
/////////////////////////////////////////////////////////////////////////
// Display.h - template display functions                              //
//           - most require use of custom type traits                  //
//                                                                     //
// Jim Fawcett, Emeritus Teaching Professor, EECS, Syracuse University //
/////////////////////////////////////////////////////////////////////////

#include "../CustomTraits/CustomTraits.h"
#include <iostream>
#include <typeinfo>

inline auto putline = [](size_t n = 1) {
  for (size_t i = 0; i < n; ++i)
    std::cout << "\n";
};

inline void displayTitle(const std::string& title)
{
  std::cout << "\n  " << title;
  std::cout << "\n " << std::string(title.size() + 2, '=') << std::endl;
}
inline void displaySubtitle(const std::string& title)
{
  std::cout << "\n  " << title;
  std::cout << "\n " << std::string(title.size() + 2, '-');
}
inline void displayDemo(const std::string& msg)
{
  std::cout << "\n  " << msg;
}

/*---- display selected type values ----*/

// https://stackoverflow.com/questions/1198260/how-can-you-iterate-over-the-elements-of-an-stdtuple

template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp), void>::type
displayTuple(std::tuple<Tp...>& t) { }

template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if < I < sizeof...(Tp), void>::type
displayTuple(std::tuple<Tp...> & t)
{
  std::cout << std::get<I>(t) << " ";
  displayTuple<I + 1, Tp...>(t);
}


template <typename T>
void displayValues(const std::initializer_list<T>& lst, const std::string& msg = "", std::string prefix = "\n  ")
{
  for (auto item : lst)
  {
    try {
      if constexpr (std::is_scalar<T>::value || Demo::is_string<T>::value)
      {
        std::cout << prefix << item;
      }
      else if constexpr (Demo::is_pair<T>::value)
      {
        std::cout << prefix << "{ " << item.first << ", " << item.second << " }";
      }
      else if constexpr (Demo::is_tuple<T>::value)
      {
        std::cout << prefix;
        displayTuple(item);
      }
      else if constexpr (Demo::is_vector<T>::value)
      {
        for (auto elem : item)
        {
          displayValues({ elem }, "", prefix);
          prefix = ", ";
        }
      }
      else if constexpr (Demo::is_unordered_map<T>::value)
      {
        for (auto elem : item)
        {
          std::cout << prefix << "{ " << elem.first << ", " << elem.second << " }";
        }
      }
      prefix = ", ";
      if (msg.size() > 0)
        std::cout << msg;
    }
    catch (std::exception & ex)
    {
      std::cout << "\n  " << ex.what();
    }
  }
}

/*---- display type sizes ----*/

template<typename T>
void displayType(T&& t, const std::string& msg = "", bool showSize = true)
{
  std::cout << "\n  ";
  if (showSize)
    std::cout << sizeof(t) << " = size of ";
  std::string typeName = typeid(t).name();
  if (typeName.size() > 75)
    typeName = typeName.substr(0, 75) + "...";
  std::cout << typeName;
  if (msg.size() > 0)
    std::cout << msg;
}

template<typename T>
void displayOnlyType(const T& t, const std::string& msg = "")
{
  std::cout << "\n  ";
  std::string typeName = typeid(t).name();
  if (typeName.size() > 75)
    typeName = typeName.substr(0, 75) + "...";
  std::cout << typeName;
  if (msg.size() > 0)
    std::cout << msg;
}

template<typename T>
void displayTypeArgument(const std::string& msg = "", bool showSize = true) {
  std::cout << "\n  ";
  if (showSize)
    std::cout << sizeof(T) << " = size of ";
  std::string typeName = typeid(T).name();
  if (typeName.size() > 75)
    typeName = typeName.substr(0, 75) + "...";
  std::cout << typeName;
  if (msg.size() > 0)
    std::cout << msg;
}

/*--- variadic display function ---*/

// Template specialization that stops recursive evaluation

template<typename T>
void displayValues(T t)
{
  displayValues({ t });
  std::cout << "\n";
}

// Recursive definition of template function
// Args is a "parameter pack
// https://en.cppreference.com/w/cpp/language/parameter_pack

template<typename T, typename... Args>
void displayValues(T t, Args... args)
{
  displayValues({ t });
  displayValues(args...);
}
