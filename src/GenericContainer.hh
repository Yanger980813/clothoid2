/*--------------------------------------------------------------------------*\
 |                                                                          |
 |  Copyright (C) 2013                                                      |
 |                                                                          |
 |         , __                 , __                                        |
 |        /|/  \               /|/  \                                       |
 |         | __/ _   ,_         | __/ _   ,_                                |
 |         |   \|/  /  |  |   | |   \|/  /  |  |   |                        |
 |         |(__/|__/   |_/ \_/|/|(__/|__/   |_/ \_/|/                       |
 |                           /|                   /|                        |
 |                           \|                   \|                        |
 |                                                                          |
 |      Enrico Bertolazzi                                                   |
 |      Dipartimento di Ingegneria Industriale                              |
 |      Universita` degli Studi di Trento                                   |
 |      email: enrico.bertolazzi@unitn.it                                   |
 |                                                                          |
\*--------------------------------------------------------------------------*/

//
// file: GenericContainer.hh
//

#ifndef GENERIC_CONTAINER_HH
#define GENERIC_CONTAINER_HH

#include <iostream>
#include <string>
#include <complex>
#include <map>
#include <deque>
#include <vector>
#include <sstream>
#include <stdexcept>


#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
  #define GENERIC_CONTAINER_ON_WINDOWS
  //#pragma comment(lib, "kernel32.lib")
  //#pragma comment(lib, "user32.lib")
#endif

// workaround for gcc < 4.9 that do not support regex
#ifdef __GNUC__
  #if __GNUC__ > 5 || ( __GNUC__== 5 && __GNUC_MINOR__ > 8 )
  #else
    #ifndef DO_NOT_USE_CXX11
      #define DO_NOT_USE_CXX11
    #endif
  #endif
#endif

// if C++ < C++11 define nullptr
#ifdef GENERIC_CONTAINER_ON_WINDOWS
  #if _MSC_VER >= 1900
    #ifndef DO_NOT_USE_CXX11
      #define GENERIC_CONTAINER_USE_CXX11
    #endif
  #else
    #include <cstdlib>
    #ifndef nullptr
      #include <cstddef>
      #ifndef nullptr
        #define nullptr NULL
      #endif
    #endif
  #endif
#else
  #pragma clang diagnostic ignored "-Wc++98-compat"
  #if __cplusplus > 199711L
    #ifndef DO_NOT_USE_CXX11
      #define GENERIC_CONTAINER_USE_CXX11
    #endif
  #else
    #include <cstdlib>
    #ifndef nullptr
      #include <cstddef>
      #ifndef nullptr
        #define nullptr NULL
      #endif
    #endif
  #endif
#endif

#ifndef GC_DO_ERROR
  #define GC_DO_ERROR(MSG) {                           \
    std::ostringstream ost ;                           \
    ost << "in GenericContainer: " << MSG << '\n' ;    \
    GenericContainer::exception( ost.str().c_str() ) ; \
  }
#endif

#ifndef GC_ASSERT
  #define GC_ASSERT(COND,MSG) if ( !(COND) ) GC_DO_ERROR(MSG)
#endif

#ifndef GC_WARNING
  #define GC_WARNING(COND,MSG)                                 \
    if ( !(COND) ) {                                           \
      std::cout << "On line: " << __LINE__                     \
                << " file: " << __FILE__                       \
                << " in GenericContainer\nWARNING: " << MSG << '\n' ;  \
    }
#endif

#ifndef GENERIC_CONTAINER_API_DLL
  #ifdef GENERIC_CONTAINER_ON_WINDOWS
    #ifdef GENERIC_CONTAINER_EXPORT
      #define GENERIC_CONTAINER_API_DLL __declspec(dllexport)
    #elif defined(GENERIC_CONTAINER_IMPORT)
      #define GENERIC_CONTAINER_API_DLL __declspec(dllimport)
    #else
      #define GENERIC_CONTAINER_API_DLL
    #endif
  #else
    #define GENERIC_CONTAINER_API_DLL
  #endif
#endif
    
#ifdef __GNUC__
  #define GC_NO_RETURN __attribute__ ((noreturn))
#else
  #define GC_NO_RETURN
#endif
    
namespace GenericContainerNamespace {

  class GenericContainer ;

  typedef void*                   pointer_type ; //!< generic pointer type
  typedef bool                    bool_type    ; //!< boolean type data
  typedef int                     int_type     ; //!< integer type data
  typedef long                    long_type    ; //!< long integer type data
  typedef double                  real_type    ; //!< floating point type data
  typedef std::complex<real_type> complex_type ; //!< complex floating point type data
  typedef std::string             string_type  ; //!< string type data

  typedef std::vector<pointer_type> vec_pointer_type ; //!< vector of generic pointer
  typedef std::vector<bool_type>    vec_bool_type    ; //!< vector of boolean
  typedef std::vector<int_type>     vec_int_type     ; //!< vector of integer
  typedef std::vector<real_type>    vec_real_type    ; //!< vector of floating point
  typedef std::vector<complex_type> vec_complex_type ; //!< vector of complex floating point
  typedef std::vector<string_type>  vec_string_type  ; //!< vector of strings

  typedef std::vector<GenericContainer>          vector_type ; //!< vector of `GenericContainer`
  typedef std::map<string_type,GenericContainer> map_type    ; //!< associative array of `GenericContainer`

  // ---------------------------------------------------------------------------

  class mat_real_type : public vec_real_type {
    unsigned _numRows ;
    unsigned _numCols ;
  public:

    mat_real_type()
    : _numRows(0)
    , _numCols(0)
    {}

    mat_real_type( unsigned nr, unsigned nc )
    : _numRows(nr)
    , _numCols(nc)
    { vec_real_type::resize(size_type(nr*nc)) ; }

    void
    resize( unsigned nr, unsigned nc ) {
      _numRows = nr ;
      _numCols = nc ;
      vec_real_type::resize(size_type(nr*nc)) ;
    }

    unsigned numRows() const { return _numRows ; }
    unsigned numCols() const { return _numCols ; }

    real_type const & operator () ( unsigned i, unsigned j ) const ;
    real_type       & operator () ( unsigned i, unsigned j ) ;

    void info( std::basic_ostream<char> & stream ) const ;
  } ;

  // ---------------------------------------------------------------------------

  class mat_complex_type : public vec_complex_type {
    unsigned _numRows ;
    unsigned _numCols ;
  public:

    mat_complex_type()
    : _numRows(0)
    , _numCols(0)
    {}

    mat_complex_type( unsigned nr, unsigned nc )
    : _numRows(nr)
    , _numCols(nc)
    { vec_complex_type::resize(size_type(nr*nc)) ; }

    void
    resize( unsigned nr, unsigned nc ) {
      _numRows = nr ;
      _numCols = nc ;
      vec_complex_type::resize(size_type(nr*nc)) ;
    }

    unsigned numRows() const { return _numRows ; }
    unsigned numCols() const { return _numCols ; }

    complex_type const & operator () ( unsigned i, unsigned j ) const ;
    complex_type       & operator () ( unsigned i, unsigned j ) ;

    void info( std::basic_ostream<char> & stream ) const ;

  } ;

  // ---------------------------------------------------------------------------

  std::ostream & operator << ( std::ostream & s, vec_pointer_type const & v ) ;
  std::ostream & operator << ( std::ostream & s, vec_bool_type const & v ) ;
  std::ostream & operator << ( std::ostream & s, vec_int_type const & v ) ;
  std::ostream & operator << ( std::ostream & s, vec_real_type const & ) ;
  std::ostream & operator << ( std::ostream & s, vec_complex_type const & ) ;
  std::ostream & operator << ( std::ostream & s, mat_real_type const & ) ;
  std::ostream & operator << ( std::ostream & s, mat_complex_type const & ) ;

  // ---------------------------------------------------------------------------

  //! Type allowed for the `GenericContainer`
  enum TypeAllowed {
    // simple type
    GC_NOTYPE=0,
    GC_POINTER,
    GC_BOOL,
    GC_INTEGER,
    GC_LONG,
    GC_REAL,
    GC_COMPLEX,
    GC_STRING,

    // vector type
    GC_VEC_POINTER,
    GC_VEC_BOOL,
    GC_VEC_INTEGER,
    GC_VEC_REAL,
    GC_VEC_COMPLEX,
    GC_VEC_STRING,

    // matrix type
    GC_MAT_REAL,
    GC_MAT_COMPLEX,

    // complex type
    GC_VECTOR,
    GC_MAP
  } ;

  /*!
    \brief `GenericContainer` is a class which permit to store eterogeneous data:

    - pointer
    - boolean
    - integer
    - long integer
    - floating point
    - complex floating point
    - string
    - vector of pointer
    - vector of boolean
    - vector of integer
    - vector of floating point
    - vector of complex floating point
    - matrix of floating point
    - matrix of complex floating point
    - vector of string

    in addition to this data type the following two container are added

    - vector of `GenericContainer`
    - map of `GenericContainer`

   */
  class GenericContainer {
  private:

    //! Data is stored in a union
    typedef union {
      pointer_type     p ;
      bool_type        b ;
      int_type         i ;
      long_type        l ;
      real_type        r ;
      complex_type     * c ;
      string_type      * s ;

      vec_pointer_type * v_p ;
      vec_bool_type    * v_b ;
      vec_int_type     * v_i ;
      vec_real_type    * v_r ;
      vec_complex_type * v_c ;
      mat_real_type    * m_r ;
      mat_complex_type * m_c ;
      vec_string_type  * v_s ;

      vector_type      * v ;
      map_type         * m ;

    } DataStorage ;

    DataStorage _data      ; //!< The data stored in the class instance
    TypeAllowed _data_type ; //!< The kind of data stored

    void allocate_string() ;
    void allocate_complex() ;

    void allocate_vec_pointer( unsigned sz ) ;
    void allocate_vec_bool( unsigned sz ) ;
    void allocate_vec_int( unsigned sz ) ;
    void allocate_vec_real( unsigned sz ) ;
    void allocate_vec_complex( unsigned sz ) ;
    void allocate_mat_real( unsigned nr, unsigned nc ) ;
    void allocate_mat_complex( unsigned nr, unsigned nc ) ;
    void allocate_vec_string( unsigned sz ) ;

    void allocate_vector( unsigned sz ) ;
    void allocate_map() ;

    void ck(char const [],TypeAllowed) const ;
    int  ck(TypeAllowed) const ;
    void ck_or_set(char const [], TypeAllowed) ;

    void initialize() { _data_type = GC_NOTYPE ; }

    #ifdef GENERIC_CONTAINER_ON_WINDOWS
    bool simple_data()     const ;
    bool simple_vec_data() const ;
    #else
    bool simple_data()     const { return _data_type <= GC_STRING     ; }
    bool simple_vec_data() const { return _data_type <= GC_VEC_STRING ; }
    #endif

  public:

    //! build an instance of `GenericContainer` with empty data
    GENERIC_CONTAINER_API_DLL GenericContainer() ;

    //! destroy the instance of `GenericContainer`
    GENERIC_CONTAINER_API_DLL ~GenericContainer() { clear() ; }

    //! free memory of the data stored in `GenericContainer`, data type become `NOTYPE`
    GENERIC_CONTAINER_API_DLL void clear() ;
    
    //! \name Initialize simple data
    //@{
    //! Set data to `pointer_type` initialize and return a reference to the data
    GENERIC_CONTAINER_API_DLL pointer_type & set_pointer( pointer_type value );

    //! Free pointer to memory pointed, set data to `NO TYPE` initialize and return a reference to the data
    GENERIC_CONTAINER_API_DLL GenericContainer & free_pointer() ;

    //! Set data to `bool_type` initialize and return a reference to the data
    GENERIC_CONTAINER_API_DLL bool_type & set_bool( bool_type value ) ;

    //! Set data to `int_type` initialize and return a reference to the data
    GENERIC_CONTAINER_API_DLL int_type & set_int( int_type value ) ;

    //! Set data to `int_type` initialize and return a reference to the data
    GENERIC_CONTAINER_API_DLL long_type & set_long( long_type value ) ;

    //! Set data to `real_type` initialize and return a reference to the data
    GENERIC_CONTAINER_API_DLL real_type & set_real( real_type value ) ;

    //! Set data to `complex_type` initialize and return a reference to the data
    GENERIC_CONTAINER_API_DLL complex_type & set_complex( complex_type & value ) ;

    //! Set data to `complex_type` initialize and return a reference to the data
    GENERIC_CONTAINER_API_DLL complex_type & set_complex( real_type r, real_type i ) ;

    //! Set data to `string_type`, allocate and initialize. Return a reference to the data
    GENERIC_CONTAINER_API_DLL string_type & set_string( string_type const & value ) ;
    //@}

    //! \name Initialize vector data
    //@{
    /*! \brief 
        Set data to `vec_pointer_type`, allocate and initialize.
        Return a reference to vector of pointer.
        If `sz` > 0 then the vector is allocated to size `sz`.
     */
    GENERIC_CONTAINER_API_DLL vec_pointer_type & set_vec_pointer( unsigned sz = 0 ) ;

    /*! \brief
     Set data to `vec_pointer_type`, allocate and initialize.
     Return a reference to vector of pointer.
     Copy the data from vector `v`.
     */
    GENERIC_CONTAINER_API_DLL vec_pointer_type & set_vec_pointer( vec_pointer_type const & v ) ;

    /*! \brief
        Set data to `vec_bool_type`, allocate and initialize.
        Return a reference to vector of booleans.
        If `sz` > 0 then the vector is allocated to size `sz`.
     */
    GENERIC_CONTAINER_API_DLL vec_bool_type & set_vec_bool( unsigned sz = 0 ) ;

    /*! \brief
     Set data to `vec_bool_type`, allocate and initialize.
     Return a reference to vector of `bool`.
     Copy the data from vector `v`.
     */
    GENERIC_CONTAINER_API_DLL vec_bool_type & set_vec_bool( vec_bool_type const & v ) ;

    /*! \brief
        Set data to `vec_int_type`, allocate and initialize.
        Return a reference to vector of integers.
        If `sz` > 0 then the vector is allocated to size `sz`.
     */
    GENERIC_CONTAINER_API_DLL vec_int_type & set_vec_int( unsigned sz = 0 ) ;

    /*! \brief
     Set data to `vec_int_type`, allocate and initialize.
     Return a reference to vector of integer.
     Copy the data from vector `v`.
     */
    GENERIC_CONTAINER_API_DLL vec_int_type & set_vec_int( vec_int_type const & v ) ;

    /*! \brief
        Set data to `vec_real_type`, allocate and initialize.
        Return a reference to vector of floating point numbers.
     If `sz` > 0 then the vector is allocated to size `sz`.
     */
    GENERIC_CONTAINER_API_DLL vec_real_type & set_vec_real( unsigned sz = 0 ) ;

    /*! \brief
     Set data to `vec_real_type`, allocate and initialize.
     Return a reference to vector of floating point number.
     Copy the data from vector `v`.
     */
    GENERIC_CONTAINER_API_DLL vec_real_type & set_vec_real( vec_real_type const & v ) ;


    /*! \brief
        Set data to `vec_complex_type`, allocate and initialize.
        Return a reference to vector of complex floating point numbers.
     If `sz` > 0 then the vector is allocated to size `sz`.
     */
    GENERIC_CONTAINER_API_DLL vec_complex_type & set_vec_complex( unsigned sz = 0 ) ;

    /*! \brief
     Set data to `vec_complex_type`, allocate and initialize.
     Return a reference to vector of complex floating point number.
     Copy the data from vector `v`.
     */
    GENERIC_CONTAINER_API_DLL vec_complex_type & set_vec_complex( vec_complex_type const & v ) ;

    /*! \brief
        Set data to `mat_real_type`, allocate and initialize.
        Return a reference to a matrix of floating point numbers.
     If `nr` > 0 and `nc` > 0 then the matrix is allocated to size `nr` x `nc`.
     */
    GENERIC_CONTAINER_API_DLL mat_real_type & set_mat_real( unsigned nr = 0, unsigned nc = 0 ) ;

    /*! \brief
     Set data to `mat_real_type`, allocate and initialize.
     Return a reference to a matrix of floating point number.
     Copy the data from matrix `m`.
     */
    GENERIC_CONTAINER_API_DLL mat_real_type & set_mat_real( mat_real_type const & m ) ;

    /*! \brief
        Set data to `mat_complex_type`, allocate and initialize.
        Return a reference to a matrix of complex floating point numbers.
     If `nr` > 0 and `nc` > 0 then the matrix is allocated to size `nr` x `nc`.
     */
    GENERIC_CONTAINER_API_DLL mat_complex_type & set_mat_complex( unsigned nr = 0, unsigned nc = 0 ) ;

    /*! \brief
     Set data to `mat_complex_type`, allocate and initialize.
     Return a reference to a matrix of floating point number.
     Copy the data from matrix `m`.
     */
    GENERIC_CONTAINER_API_DLL mat_complex_type & set_mat_complex( mat_complex_type const & m ) ;

    /*! \brief
        Set data to `vec_string_type`, allocate and initialize.
        Return a reference to vector of strings.
        If `sz` > 0 then the vector is allocated to size `sz`.
     */
    GENERIC_CONTAINER_API_DLL vec_string_type & set_vec_string( unsigned sz = 0 ) ;
    
    /*! \brief
     Set data to `vec_string_type`, allocate and initialize.
     Return a reference to vector of strings.
     Copy the data from vector `v`.
     */
    GENERIC_CONTAINER_API_DLL vec_string_type & set_vec_string( vec_string_type const & v ) ;

    //! push boolean if data is `vec_bool_type` or `vector_type'
    GENERIC_CONTAINER_API_DLL void push_bool( bool ) ;

    //! push integer if data is `vec_int_type` or `vector_type'
    GENERIC_CONTAINER_API_DLL void push_int( int_type ) ;

    //! push integer if data is `vec_int_type` or `vector_type'
    GENERIC_CONTAINER_API_DLL void push_long( long_type ) ;

    //! push real if data is `vec_real_type` or `vector_type'
    GENERIC_CONTAINER_API_DLL void push_real( real_type ) ;

    //! push complex if data is `vec_complex_type` or `vector_type'
    GENERIC_CONTAINER_API_DLL void push_complex( complex_type & ) ;

    //! push complex if data is `vec_complex_type` or `vector_type'
    GENERIC_CONTAINER_API_DLL void push_complex( real_type re, real_type im ) ;

    //! push complex if data is `vec_string_type` or `vector_type'
    GENERIC_CONTAINER_API_DLL void push_string( string_type const & ) ;

    //@}

    //! \name Initialize generic data
    //@{
    /*! \brief 
        Set data to `vector_type`, allocate an empty generic vector and return a reference to it.
        If `sz` > 0 then the vector is allocated to size `sz`.
     */
    vector_type & set_vector( unsigned sz = 0 ) ;
    
    //! Set data to `map_type`, allocate an empty generic map and return a reference to it.
    GENERIC_CONTAINER_API_DLL map_type & set_map() ;
    //@}
    
    //! \name Access to a single element
    //@{
    
    //! Return an integer representing the type of data stored
    /*!
       Integer to data type map
       ------------------------
   
         -   No data stored (return 0)
         1.  `pointer_type`
         2.  `bool_type`
         3.  `int_type`
         4.  `long_type`
         5.  `real_type`
         6.  `complex_type`
         7.  `string_data`
         8.  `vec_pointer_type`
         9.  `vec_bool_type`
         10. `vec_int_type`
         11. `vec_real_type`
         12. `vec_complex_type`
         13. `vec_string_type`
         14. `mat_real_type`
         15. `mat_complex_type`
         16. `vector_type`
         17. `map_type`

    */
    TypeAllowed get_type() const { return _data_type ; }
    
    //! Return a string pointer representing the type of data stored
    GENERIC_CONTAINER_API_DLL char const * get_type_name() const ;

    //! Print to stream the kind of data stored
    GENERIC_CONTAINER_API_DLL GenericContainer const & info( std::basic_ostream<char> & stream ) const ;

    /*! Return the number of the elements of the first level of the generic container
    //  1 for single element, the size of vector or map, or 0
    */
    GENERIC_CONTAINER_API_DLL unsigned get_num_elements() const ;
    GENERIC_CONTAINER_API_DLL unsigned get_numRows() const ;
    GENERIC_CONTAINER_API_DLL unsigned get_numCols() const ;

    //! If data is boolean, integer or floating point return number, otherwise return `0`.
    GENERIC_CONTAINER_API_DLL real_type    get_number() const ;
    GENERIC_CONTAINER_API_DLL complex_type get_complex_number() const ;
    GENERIC_CONTAINER_API_DLL void         get_complex_number( real_type & re, real_type & im ) const ;

    GENERIC_CONTAINER_API_DLL void *  get_pvoid( char const msg[] = nullptr ) const ;
    GENERIC_CONTAINER_API_DLL void ** get_ppvoid( char const msg[] = nullptr ) const ;

    #ifdef GENERIC_CONTAINER_ON_WINDOWS
    template <typename T>
    T& get_pointer()
    { ck("get_pointer",GC_POINTER) ; return *static_cast<T*>(get_ppvoid()) ; }

    template <typename T>
    T get_pointer() const
    { ck("get_pointer",GC_POINTER) ; return static_cast<T>(get_pvoid()) ; }
    #else
    template <typename T>
    T& get_pointer()
    { ck("get_pointer",GC_POINTER) ; return *static_cast<T*>(&(_data.p)) ; }

    template <typename T>
    T get_pointer() const
    { ck("get_pointer",GC_POINTER) ; return static_cast<T>(_data.p) ; }
    #endif

    GENERIC_CONTAINER_API_DLL bool_type       & get_bool( char const msg[] = nullptr ) ;
    GENERIC_CONTAINER_API_DLL bool_type const & get_bool( char const msg[] = nullptr ) const ;
    //!< Return the stored boolean (if fails issue an error).

    GENERIC_CONTAINER_API_DLL int_type       & get_int( char const msg[] = nullptr ) ;
    GENERIC_CONTAINER_API_DLL int_type const & get_int( char const msg[] = nullptr ) const ;
    //!< Return the stored integer (if fails issue an error).

    GENERIC_CONTAINER_API_DLL long_type       & get_long( char const msg[] = nullptr ) ;
    GENERIC_CONTAINER_API_DLL long_type const & get_long( char const msg[] = nullptr ) const ;
    //!< Return the stored long integer (if fails issue an error).

    GENERIC_CONTAINER_API_DLL real_type       & get_real( char const msg[] = nullptr ) ;
    GENERIC_CONTAINER_API_DLL real_type const & get_real( char const msg[] = nullptr ) const ;
    //!< Return the stored floating point (if fails issue an error).

    GENERIC_CONTAINER_API_DLL complex_type       & get_complex( char const msg[] = nullptr ) ;
    GENERIC_CONTAINER_API_DLL complex_type const & get_complex( char const msg[] = nullptr ) const ;
    //!< Return the stored complex floating point (if fails issue an error).

    GENERIC_CONTAINER_API_DLL string_type       & get_string( char const msg[] = nullptr ) ;
    GENERIC_CONTAINER_API_DLL string_type const & get_string( char const msg[] = nullptr ) const ;
    //!< Return the stored string (if fails issue an error).
    //@}

    //! \name Access to vector type data
    //@{
    GENERIC_CONTAINER_API_DLL vector_type       & get_vector( char const msg[] = nullptr ) ;
    GENERIC_CONTAINER_API_DLL vector_type const & get_vector( char const msg[] = nullptr ) const ;
    //!< Return reference to a generic vector (if fails issue an error).

    GENERIC_CONTAINER_API_DLL vec_pointer_type       & get_vec_pointer( char const msg[] = nullptr ) ;
    GENERIC_CONTAINER_API_DLL vec_pointer_type const & get_vec_pointer( char const msg[] = nullptr ) const ;
    //!< Return reference to a vector of pointer (if fails issue an error).
    
    GENERIC_CONTAINER_API_DLL vec_bool_type       & get_vec_bool( char const msg[] = nullptr ) ;
    GENERIC_CONTAINER_API_DLL vec_bool_type const & get_vec_bool( char const msg[] = nullptr ) const ;
    //!< Return reference to a vector of booleans (if fails issue an error).

    GENERIC_CONTAINER_API_DLL vec_int_type       & get_vec_int( char const msg[] = nullptr ) ;
    GENERIC_CONTAINER_API_DLL vec_int_type const & get_vec_int( char const msg[] = nullptr ) const ;
    //!< Return reference to a vector of integers (if fails issue an error).

    GENERIC_CONTAINER_API_DLL vec_real_type       & get_vec_real( char const msg[] = nullptr ) ;
    GENERIC_CONTAINER_API_DLL vec_real_type const & get_vec_real( char const msg[] = nullptr ) const ;
    //!< Return reference to a vector of floating point number (if fails issue an error).

    GENERIC_CONTAINER_API_DLL vec_complex_type       & get_vec_complex( char const msg[] = nullptr ) ;
    GENERIC_CONTAINER_API_DLL vec_complex_type const & get_vec_complex( char const msg[] = nullptr ) const ;
    //!< Return reference to a vector of complex floating point number (if fails issue an error).

    GENERIC_CONTAINER_API_DLL mat_real_type       & get_mat_real( char const msg[] = nullptr ) ;
    GENERIC_CONTAINER_API_DLL mat_real_type const & get_mat_real( char const msg[] = nullptr ) const ;
    //!< Return reference to a matrix of floating point number (if fails issue an error).

    GENERIC_CONTAINER_API_DLL mat_complex_type       & get_mat_complex( char const msg[] = nullptr ) ;
    GENERIC_CONTAINER_API_DLL mat_complex_type const & get_mat_complex( char const msg[] = nullptr ) const ;
    //!< Return reference to a matrix of complex floating point number (if fails issue an error).

    GENERIC_CONTAINER_API_DLL vec_string_type       & get_vec_string( char const msg[] = nullptr ) ;
    GENERIC_CONTAINER_API_DLL vec_string_type const & get_vec_string( char const msg[] = nullptr ) const ;
    //!< Return reference to a vector of strings (if fails issue an error).
    //@}

    //! \name Access to element of vector type data
    //@{
    //! If `i`-th element of the vector is boolean, integer or floating point then return number, otherwise return `0`.
    GENERIC_CONTAINER_API_DLL real_type    get_number_at( unsigned i ) const ;
    GENERIC_CONTAINER_API_DLL complex_type get_complex_number_at( unsigned i ) const ;
    GENERIC_CONTAINER_API_DLL void         get_complex_number_at( unsigned i, real_type & re, real_type & im ) const ;

    template <typename T>
    T& get_pointer_at( unsigned i )
    { return (*this)[i].get_pointer<T>() ; }

    template <typename T>
    T get_pointer_at( unsigned i ) const
    { return (*this)[i].get_pointer<T>() ; }
    //!< Return `i`-th generic pointer (if fails issue an error).

    GENERIC_CONTAINER_API_DLL bool_type get_bool_at( unsigned i ) ;
    GENERIC_CONTAINER_API_DLL bool_type get_bool_at( unsigned i, char const msg[] ) const ;
    //!< Return `i`-th boolean (if fails issue an error).

    GENERIC_CONTAINER_API_DLL int_type       & get_int_at( unsigned i ) ;
    GENERIC_CONTAINER_API_DLL int_type const & get_int_at( unsigned i, char const msg[] ) const ;
    //!< Return `i`-th integer (if fails issue an error).

    GENERIC_CONTAINER_API_DLL real_type       & get_real_at( unsigned i ) ;
    GENERIC_CONTAINER_API_DLL real_type const & get_real_at( unsigned i, char const msg[] ) const ;
    //!< Return `i`-th floating point number (if fails issue an error).

    GENERIC_CONTAINER_API_DLL complex_type       & get_complex_at( unsigned i ) ;
    GENERIC_CONTAINER_API_DLL complex_type const & get_complex_at( unsigned i, char const msg[] ) const ;
    //!< Return `i`-th complex floating point number (if fails issue an error).

    GENERIC_CONTAINER_API_DLL real_type       & get_real_at( unsigned i, unsigned j ) ;
    GENERIC_CONTAINER_API_DLL real_type const & get_real_at( unsigned i, unsigned j, char const msg[] ) const ;
    //!< Return `i`-th floating point number (if fails issue an error).

    GENERIC_CONTAINER_API_DLL complex_type       & get_complex_at( unsigned i, unsigned j ) ;
    GENERIC_CONTAINER_API_DLL complex_type const & get_complex_at( unsigned i, unsigned j, char const msg[] ) const ;
    //!< Return `i`-th complex floating point number (if fails issue an error).

    GENERIC_CONTAINER_API_DLL string_type       & get_string_at( unsigned i ) ;
    GENERIC_CONTAINER_API_DLL string_type const & get_string_at( unsigned i, char const msg[] ) const ;
    //!< Return `i`-th string (if fails issue an error).

    GENERIC_CONTAINER_API_DLL GenericContainer       & get_gc_at( unsigned i ) ;
    GENERIC_CONTAINER_API_DLL GenericContainer const & get_gc_at( unsigned i, char const msg[] ) const ;
    //!< Return `i`-th `GenericContainer` of a generic vector (if fails issue an error).
    //@}

    //! \name Access to map type element
    //@{
    GENERIC_CONTAINER_API_DLL map_type       & get_map( char const msg[] = nullptr ) ;
    GENERIC_CONTAINER_API_DLL map_type const & get_map( char const msg[] = nullptr ) const ;
    //!< Return the reference of the stored map or issue an error.

    //! Check if string `s` is a key of the stored map (if fails issue an error).
    GENERIC_CONTAINER_API_DLL bool exists( std::string const & s ) const ;
    //@}

    //! \name Access using operators
    //@{
    
    GENERIC_CONTAINER_API_DLL GenericContainer       & operator [] ( unsigned i ) ;
    GENERIC_CONTAINER_API_DLL GenericContainer const & operator [] ( unsigned i ) const ;
    /*! \brief
        Overload of the `[]` operator to access the `i`-th element of a stored generic vector.
        Run time bound check.
     */

    GENERIC_CONTAINER_API_DLL GenericContainer       & operator [] ( std::string const & s ) ;
    GENERIC_CONTAINER_API_DLL GenericContainer const & operator [] ( std::string const & s ) const ;
    /*! \brief
       Overload of the `[]` operator to access the `s`-th element of a stored generic map. 
       If the element do not exists it is created.
    */
    
    GENERIC_CONTAINER_API_DLL GenericContainer       & operator () ( unsigned i, char const msg[] = nullptr ) ;
    GENERIC_CONTAINER_API_DLL GenericContainer const & operator () ( unsigned i, char const msg[] = nullptr ) const ;
    
    /*! \brief
     Overload of the `()` operator to access the `i`-th element of a stored generic vector.
     NO run time bound check.
     */

    GENERIC_CONTAINER_API_DLL GenericContainer       & operator () ( std::string const & s, char const msg[] = nullptr ) ;
    GENERIC_CONTAINER_API_DLL GenericContainer const & operator () ( std::string const & s, char const msg[] = nullptr ) const ;

    /*! \brief
       Overload of the `()` operator to access the `s`-th element of a stored generic map.
       If the element do not exists an error is issued.
     */

    //@}
    
    //! \name Initialize data using operators
    //! The `=` operator is overloaded to initialize the `GenericContainer` on its left side.
    //@{

    //! Assign a boolean to the generic container.
    GenericContainer & operator = ( bool a )
    { this -> set_bool(a) ; return * this ; }

    //! Assign an integer to the generic container.
    GenericContainer & operator = ( unsigned a )
    { this -> set_int(int_type(a)) ; return * this ; }

    //! Assign an integer to the generic container.
    GenericContainer & operator = ( int_type a )
    { this -> set_int(a) ; return * this ; }

    //! Assign an integer to the generic container.
    GenericContainer & operator = ( unsigned long a )
    { this -> set_long(long_type(a)) ; return * this ; }

    //! Assign an integer to the generic container.
    GenericContainer & operator = ( long_type a )
    { this -> set_long(a) ; return * this ; }

    //! Assign a floating point number to the generic container.
    GenericContainer & operator = ( float a )
    { this -> set_real(real_type(a)) ; return * this ; }

    //! Assign a floating point number to the generic container.
    GenericContainer & operator = ( double a )
    { this -> set_real(real_type(a)) ; return * this ; }

    //! Assign a floating point number to the generic container.
    GenericContainer & operator = ( std::complex<float> & a )
    { this -> set_complex(real_type(a.real()),real_type(a.imag())) ; return * this ; }

    //! Assign a floating point number to the generic container.
    GenericContainer & operator = ( std::complex<double> & a )
    { this -> set_complex(real_type(a.real()),real_type(a.imag())) ; return * this ; }

    //! Assign a string to the generic container.
    GenericContainer & operator = ( char const a[] )
    { this -> set_string(a) ; return * this ; }

    //! Assign a string to the generic container.
    GenericContainer & operator = ( std::string const & a )
    { this -> set_string(a) ; return * this ; }

    //! Assign a pointer to the generic container.
    GenericContainer & operator = ( void * a )
    { this -> set_pointer(a) ; return * this ; }

    //! Assign a generic container `a` to the generic container.
    GenericContainer const & operator = ( GenericContainer const & a )
    { this -> load( a ) ; return * this ; }

    //! Copy a generic container `a` to the generic container.
    GENERIC_CONTAINER_API_DLL void load( GenericContainer const & a ) ;
    //@}

    //! \name Promotion to a ``bigger'' data
    //@{
    //! If data contains a boolean it is promoted to an integer.
    GENERIC_CONTAINER_API_DLL GenericContainer const & promote_to_int() ;

    //! If data contains a boolean it is promoted to an integer.
    GENERIC_CONTAINER_API_DLL GenericContainer const & promote_to_long() ;

    //! If data contains a boolean or an integer it is promoted to a real.
    GENERIC_CONTAINER_API_DLL GenericContainer const & promote_to_real() ;

    //! If data contains a boolean or an integer or floating point it is promoted to a complex floating point.
    GENERIC_CONTAINER_API_DLL GenericContainer const & promote_to_complex() ;

    //! If data contains vector of booleans it is promoted to a vector of integer.
    GENERIC_CONTAINER_API_DLL GenericContainer const & promote_to_vec_int() ;

    //! If data contains vector of booleans or integer it is promoted to a vector of real.
    GENERIC_CONTAINER_API_DLL GenericContainer const & promote_to_vec_real() ;

    //! If data contains vector of booleans or integer or real it is promoted to a vector of complex.
    GENERIC_CONTAINER_API_DLL GenericContainer const & promote_to_vec_complex() ;

    //! If data contains vector of booleans, integer or real it is promoted to a matrix of real.
    GENERIC_CONTAINER_API_DLL GenericContainer const & promote_to_mat_real() ;

    //! If data contains vector of booleans, integer or real or complex or matrix of real it is promoted to a matrix of complex.
    GENERIC_CONTAINER_API_DLL GenericContainer const & promote_to_mat_complex() ;

    //! If data contains vector of someting it is promoted to a vector of `GenericContainer`.
    GENERIC_CONTAINER_API_DLL GenericContainer const & promote_to_vector() ;
    //@}

    //! \name Initialize data by overloading constructor
    //@{
    
    //! Construct a generic container storing a boolean
    GenericContainer( bool a ) { initialize() ; this->operator=(a) ; }
    
    //! Construct a generic container storing an integer
    GenericContainer( unsigned a ) { initialize() ; *this = a ; }
    
    //! Construct a generic container storing an integer
    GenericContainer( int a ) { initialize() ; this->operator=(a) ; }
    
    //! Construct a generic container storing a floating point number
    GenericContainer( float a ) { initialize() ; this->operator=(a) ; }

    //! Construct a generic container storing a floating point number
    GenericContainer( double a ) { initialize() ; this->operator=(a) ; }

    //! Construct a generic container storing a complex floating point number
    GenericContainer( std::complex<float> & a ) { initialize() ; this->operator=(a) ; }

    //! Construct a generic container storing a complex floating point number
    GenericContainer( std::complex<double> & a ) { initialize() ; this->operator=(a) ; }
    
    //! Construct a generic container storing a string
    GenericContainer( char const a[] ) { initialize() ; this->operator=(a) ; }

    //! Construct a generic container storing a string
    GenericContainer( std::string const & a ) { initialize() ; this->operator=(a) ; }
    
    //! Construct a generic container storing a pointer
    GenericContainer( void * a ) { initialize() ; this->operator=(a) ; }

    //! Construct a generic container copying container `gc`
    GenericContainer( GenericContainer const & gc ) { initialize() ; this->operator=(gc) ; }
    //@}

    //! \name I/O for `GenericContainer` objects
    //@{

    //! print the contents of the object in a human readable way
    GENERIC_CONTAINER_API_DLL
    void print( std::basic_ostream<char> &,
                std::string const & prefix = "",
                std::string const & indent = "    " ) const ;

    //! print the contents of the object in yaml syntax
    GENERIC_CONTAINER_API_DLL
    void to_yaml( std::basic_ostream<char> &, std::string const & prefix = "" ) const ;

    /*! 
      \brief write `GenericContainer` as regular formatted data
       
      Write the contents of the `GenericContainer` object to stream.
     
      `GenericContainer` must be a map which contains the fields:
       
      - "headers" this element must be a `vec_string_type` which contains
                  the strings of the headers of the columns of the data

      - "data"    this element must be a `vector_type` which contais the
                  vectors which are the columns of the data to be saved.
                  Each column can be of type
                  
                  1. `vec_bool_type`
                  2. `vec_int_type`
                  3. `vec_real_type`
     
                  all the vector must have the same size.

       \param stream     stream to write the output
       \param delimiter  desired delimiter (optional). Default is tab.
     */
    GENERIC_CONTAINER_API_DLL
    GenericContainer const &
    writeFormattedData( std::basic_ostream<char> & stream, char const delimiter = '\t' ) const ;

    /*! 
      \brief read regular formatted data from `stream` to `GenericContainer`.
     
      After successful read `GenericContainer` will be a map which contains the fields:
       
      - "headers"  a `vec_string_type` which contains
                   the strings of the headers of the columns of the data

      - "data"     a `vector_type` which contais the vectors which are the
                   columns of the data readed of type `vec_real_type`.

       \param stream       stream to write the output
       \param commentChars lines beginnig with one of this chars are treated as comments. 
                           Default are `#` and `%`
       \param delimiters   caracters used as delimiter for headers
     */
    GENERIC_CONTAINER_API_DLL
    GenericContainer &
    readFormattedData( std::istream & stream,
                       char const commentChars[] = "#%",
                       char const delimiters[] = " \t" ) ;

    GENERIC_CONTAINER_API_DLL static void exception( char const msg[] ) GC_NO_RETURN ;

  } ;

  // -------------------------------------------------------
  // support functions
  GENERIC_CONTAINER_API_DLL
  void
  writeTable( vec_string_type const    & headers,
              vector_type     const    & data,
              std::basic_ostream<char> & stream,
              char const delimiter = '\t' ) ;

  GENERIC_CONTAINER_API_DLL
  void
  writeTableFormatted( vec_string_type const    & headers,
                       vector_type     const    & data,
                       std::basic_ostream<char> & stream ) ;

  // -------------------------------------------------------
  class GenericContainerExplorer {
  private:
    enum {
      GENERIC_CONTAINER_OK        = 0,
      GENERIC_CONTAINER_BAD_TYPE  = 1,
      GENERIC_CONTAINER_NO_DATA   = 2,
      GENERIC_CONTAINER_NOT_EMPTY = 3,
      GENERIC_CONTAINER_BAD_HEAD  = 4
    } ;

    GenericContainer              data ;
    std::deque<GenericContainer*> head ;

    map_type         * ptr_map ;
    map_type::iterator map_iterator ;

  public:
  
    GENERIC_CONTAINER_API_DLL GenericContainerExplorer() { head.push_back(&data) ; }
    GENERIC_CONTAINER_API_DLL ~GenericContainerExplorer() {}

    GenericContainer *
    top() {
      GC_ASSERT( head.size() > 0, "GenericContainerExplorer::top() empty stack!" ) ;
      GC_ASSERT( head.back() != nullptr, "GenericContainerExplorer::top() bad top pointer!" ) ;
      return head.back() ;
    }

    GenericContainer const *
    top() const {
      GC_ASSERT( head.size() > 0, "GenericContainerExplorer::top() empty stack!" ) ;
      GC_ASSERT( head.back() != nullptr, "GenericContainerExplorer::top() bad top pointer!" ) ;
      return head.back() ;
    }

    void       * mem_ptr()       { return &data ; }
    void const * mem_ptr() const { return &data ; }

    int
    check( int data_type ) const {
      if ( head.empty() ) return GENERIC_CONTAINER_BAD_HEAD ;
      if ( data_type == head.back() -> get_type() ) {
        if ( head.back() == nullptr ) return GENERIC_CONTAINER_NO_DATA ;
        return GENERIC_CONTAINER_OK ;
      } else {
        return GENERIC_CONTAINER_BAD_TYPE ;
      }
    }

    int
    check_no_data( int data_type ) const {
      if ( head.empty() ) return GENERIC_CONTAINER_BAD_HEAD ;
      if ( GC_NOTYPE == head.back() -> get_type() ||
           data_type == head.back() -> get_type() ) return GENERIC_CONTAINER_OK ;
      return GENERIC_CONTAINER_NOT_EMPTY ;
    }

    int
    pop() {
      if ( head.empty() ) return GENERIC_CONTAINER_NO_DATA ;
      head.pop_back() ;
      return GENERIC_CONTAINER_OK ;
    }

    int
    push( GenericContainer * gc ) {
      head.push_back( gc ) ;
      return GENERIC_CONTAINER_OK ;
    }

    int
    push_vector_position( unsigned pos ) {
      int ok = check( GC_VECTOR ) ;
      if ( ok == GENERIC_CONTAINER_OK  ) {
        GenericContainer * gc = &((*head.back())[pos]) ;
        head.push_back( gc ) ;
      }
      return ok ;
    }

    int
    push_map_position( char const pos[] ) {
      int ok = check( GC_MAP ) ;
      if ( ok == GENERIC_CONTAINER_OK  ) {
        GenericContainer * gc = &((*head.back())[pos]) ;
        head.push_back( gc ) ;
      }
      return ok ;
    }

    int
    init_map_key() {
      int ok = check( GC_MAP ) ;
      if ( ok == GENERIC_CONTAINER_OK ) {
        ptr_map = &head.back()->get_map() ;
        map_iterator = ptr_map->begin() ;
      }
      return ok ;
    }

    char const *
    next_map_key() {
      if ( map_iterator != ptr_map->end() )
        return map_iterator++ -> first.c_str() ;
      else
        return nullptr ;
    }
  
    int
    reset() {
      if ( head.empty() ) return GENERIC_CONTAINER_NO_DATA ;
      while ( head.size() > 1 ) head.pop_back() ;
      return GENERIC_CONTAINER_OK ;
    }

  } ;
}

// do not define alias GC if use X11
#ifndef XlibSpecificationRelease
namespace GC = GenericContainerNamespace ;
#endif

#endif

//
// eof: GenericContainer.hh
//