
#include "LinkedList.h"
template <class K, class V>
class KeyValuePair
{
public:
  K key;
  V value;
  ~KeyValuePair()
  {
    delete value;
  }
  KeyValuePair(K _key, V _value) : key(_key), value(_value) {}

  bool operator==(const KeyValuePair &other)
  {
    return key == other.key;
  }
};

template <class K, class V>
class Map
{

public:
  LinkedList<KeyValuePair<K, V>> list;

  int getLength() { return list.getLength(); }
  V &operator[](const K &key)
  {
    if (list.moveToStart())
    {
      do
      {
        if (list.getCurrent().key == key)
        {
          return list.getCurrent().value;
        }
      } while (list.next());
    }

    KeyValuePair<K, V>
        newPair(key, NULL);
    list.Append(newPair);
    return list.Last().value;
  }



  void insert(const K &key, const V &value)
  {
    if (list.moveToStart())
    {
      do
      {
        if (list.getCurrent().key == key)
        {
          list.getCurrent().value = value;
          return;
        }
      } while (list.next());
    }

    list.Append(KeyValuePair<K, V>(key, value));
  }

  bool remove(const K &key)
  {
    list.moveToStart();
    do
    {
      if (list.getCurrent().key == key)
      {
        list.DeleteCurrent();
        return true;
      }
    } while (list.next());
    return false;
  }

  bool contains(const K &key) const
  {
    list.moveToStart();
    do
    {
      if (list.getCurrent().key == key)
      {
        return true;
      }
    } while (list.next());
    return false;
  }
};
