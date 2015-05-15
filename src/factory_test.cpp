#include <iostream> // std::cout
#include <memory> // For unique_ptr 

#include <boostx/factory.hpp> // For our factory
#include <boostx/type_info.hpp> // For default typeinfo and some convenience macros

/// Simple example with default boostx::type_info and an abstract with 3 sub classes
namespace first
{
  /// A first abstract interface with a virtual member function
  class abstract_interface
  {
  public:
    abstract_interface(int _bar) : 
      bar_(_bar) {}

    /// A first virtual member function
    virtual void foo() const = 0;

    /// Declares the virtual get_type_id member function which returns ""
    BOOSTX_DECL_TYPE_ID_DEFAULT("")

    /// A first member variable
    int bar_;
  };

  /// Lets define our factory.
  typedef boostx::factory<
    abstract_interface, // Interface type
    boostx::type_info, // Use default type info type
    int // The constructor signature    
  > my_factory;

  /// Derived alias template for registrar 
  template<typename T> 
  using registrar = my_factory::registrar_type<T>;

  /// A sample class "A" with auto registration
  class A : 
    public abstract_interface, // Inherits from abstract_interface
    registrar<A>  // Privately inherit from registrar for auto registration
  {
  public:
    /// Declare type id
    BOOSTX_DECL_TYPE_ID_DEFAULT("A")
  
    A(int t) : abstract_interface(t) {}

    /// foo() is outputting bar multiplied by 4
    void foo() const 
    { 
      std::cout << "A (has type id " << get_type_id() << "): " << (bar_*2) << std::endl; 
    }   
  };
  
  /// A sample class "B" with auto registration
  class B : 
    public abstract_interface, // Inherits from abstract_interface
    registrar<B> // Privately inherit from registrar for auto registration
  {
  public:
    BOOSTX_DECL_TYPE_ID_DEFAULT("B")

    B(int t) : abstract_interface(t) {}

    /// foo() is outputting bar multiplied by 3
    void foo() const 
    { 
      std::cout << "B (has type id " << get_type_id() << "): " << (bar_*3) << std::endl; 
    }   
  };

  /// Sample class "C" without auto registration
  class C: public abstract_interface
  {
  public:
    C(int t) : abstract_interface(t) {}

    /// foo () outputting bar multiplied by 4
    void foo() const 
    { 
      std::cout << "C (has type id " << get_type_id() << "): " << (bar_*4) << std::endl;
    }   
  };

  void example()
  { 
    std::cout << "***** Output of first example *****" << std::endl;
    
    /// Create an instance of A with bar = 1
    std::unique_ptr<abstract_interface> _a(my_factory::create("A",1));
    
    /// Create an instance of B with bar = 2
    std::unique_ptr<abstract_interface> _b(my_factory::create("B",2));
    
    /// Register class C with custom id
    my_factory::reg<C>("Custom");
    
    /// List all classes
    for (auto _reg : my_factory::classes())
    {
      std::cout << "TypeId: " << "\t" << _reg.first << std::endl;
    }

    /// Create an instance of C with bar = 4 
    std::unique_ptr<abstract_interface> _c(my_factory::create("Custom",3));

    _a->foo();  // Output: A (has type id A): 2  // (2 * 1)
    _b->foo();  // Output: B (has type id B): 6 // (3 * 2)
    _c->foo();  // Output: C (has type id ): 12 // (4 * 3)

    std::cout << std::endl;
  }
}

/// Second example with custom type info type and unregistration test 
namespace second
{ 
  /// Declare type id mechanism
  BOOSTX_DECL_TYPE_INFO(
      type_info, // Name of type id struct
      char, // Type id type
      typeId, // Static member function
      getTypeId // Virtual member function of abstract interface
  )

  /// Macro for declaring a type id
#define DECL_TYPE_ID(TYPEID)\
  BOOSTX_TYPE_ID(char,TYPEID,typeId,getTypeId)
  
  /// A first abstract interface with a virtual member function
  class abstract_interface
  {
  public:
    abstract_interface(
        std::string const& _str,
        int _bar) : 
      str_(_str),
      bar_(_bar) 
    {}

    /// Our abstract virtual member function 
    virtual void foo() const = 0;

    DECL_TYPE_ID(' ') 

  protected:
    std::string str_;
    int bar_;
  };

  /// Lets define our factory.
  typedef boostx::factory<
    abstract_interface, // Interface type
    type_info, // Our type info type
    std::string const&, int // The constructor signature    
  > my_factory;

  /// Derived alias template for registrar 
  template<typename T> 
  using registrar = my_factory::registrar_type<T>;

  /// Sample class A
  class A : 
    public abstract_interface,
    registrar<A>
  {
  public:
    A(std::string const& _str, int _bar) : 
      abstract_interface(_str,_bar)
    {
    }
    
    DECL_TYPE_ID('A') 

    void foo() const
    {
      std::cout << "A" << this->str_ << " " << this->bar_ << std::endl;
    }
  };
  
  /// Sample class B
  class B : 
    public abstract_interface,
    registrar<B>
  {
  public:
    B(std::string const& _str, int _bar) : 
      abstract_interface(_str,_bar)
    {
    }

    DECL_TYPE_ID('B') 
    
    void foo() const
    {
      std::cout << "B" << this->str_ << " " << this->bar_ << std::endl;
    }
  };

  void example()
  {
    std::cout << "***** Output of second example *****" << std::endl;
 
    /// List all classes
    for (auto _reg : my_factory::classes())
    {
      std::cout << "TypeId: " << "\t" << _reg.first << std::endl;
    }
    
    std::cout << "Number of registered classes: " << my_factory::classes().size() << std::endl;

    /// Create an instance of A with str = "$" and bar = 1
    std::unique_ptr<abstract_interface> _a(my_factory::create('A',"$",1));
    
    /// Create an instance of B with str = "€" and bar = 2
    std::unique_ptr<abstract_interface> _b(my_factory::create('B',"€",2));

    _a->foo(); // Output A$ 1
    _b->foo(); // Output B$ 2

    /// Unregister B
    my_factory::unreg('B');

    /// Test unregister mechanism
    if (!my_factory::exists('B'))
    {
      std::cout << "There is no class B anymore" << std::endl;
    }

    my_factory::unreg<A>();
    /// Test unregister mechanism
    if (!my_factory::exists('A'))
    {
      std::cout << "There is no class A anymore" << std::endl;
    }

    std::cout << "Number of registered classes: " << my_factory::classes().size() << std::endl;

    std::cout << std::endl;
  }
}


int main(int argc, char* argv[])
{
  /// Short and first example with 3 classes with default type info mechanism
  first::example();

  /// Second example with custom type info type and unregistration test 
  second::example();

  return EXIT_SUCCESS;
}
