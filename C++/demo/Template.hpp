template <class TDescriptor, class F> //模板类
/// Generic Database
class TemplatedDatabase
{
    template <class T> //模板函数
    void setVocabulary(const T &voc, bool use_di, int di_levels = 0);
};

template <class TDescriptor, class F>
template <class T>
inline void TemplatedDatabase<TDescriptor, F>::setVocabulary(const T &voc, bool use_di, int di_levels)
{
    m_use_di = use_di;
    m_dilevels = di_levels;
    delete m_voc;
    m_voc = new T(voc);
    clear();
}