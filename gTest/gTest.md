# primer
## Assertions
在 assertions 失败时， gtest 会在失败消息后打印文件名和行号 
- `ASSERT_*`失败时，会产生`fatal failures`，并终止当前测试函数
- `EXPECT_*`失败时，会产生`nonfatal failures`，但不会终止当前测试函数

- `ASSERT_*`失败时会立即返回，可能会跳过后续的内存清理代码，从而导致内春的泄漏。（如果出现堆检查错误时，需要考虑到这一点）

## Test Fixtures: Using the Same Data Configuration for Multiple Tests
使用相同的数据或配置进行不同的测试
To create a fixture:
```text
1. 定以 ::testing::Test 的派生类，并从 protected 开始 body
2. 在类中声明需要的用到的对象
3. 如果需要，编写 SetUp() 构找函数，在其中准备需要测试的数据，
4. 如果需要，TearDown() 析构函数，释放资源
5. 如果需要，定义共享的测试子流程
```
## Writing the main() Function
大多数情况下，用户不需要写自己的 main 函数，而是在 link 时使用 gtest_main 代替

```C++
#include "this/package/foo.h"

#include "gtest/gtest.h"

namespace my {
namespace project {
namespace {

// The fixture for testing class Foo.
class FooTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  FooTest() {
     // You can do set-up work for each test here.
  }

  ~FooTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Class members declared here can be used by all tests in the test suite
  // for Foo.
};

// Tests that the Foo::Bar() method does Abc.
TEST_F(FooTest, MethodBarDoesAbc) {
  const std::string input_filepath = "this/package/testdata/myinputfile.dat";
  const std::string output_filepath = "this/package/testdata/myoutputfile.dat";
  Foo f;
  EXPECT_EQ(f.Bar(input_filepath, output_filepath), 0);
}

// Tests that Foo does Xyz.
TEST_F(FooTest, DoesXyz) {
  // Exercises the Xyz feature of Foo.
}

}  // namespace
}  // namespace project
}  // namespace my

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
```

## Floating-Point Comparison
```C++
using ::testing::FloatLE;
using ::testing::DoubleLE;
...
EXPECT_PRED_FORMAT2(FloatLE, val1, val2);
EXPECT_PRED_FORMAT2(DoubleLE, val1, val2);
```
## More String Assertions
```C++
using ::testing::HasSubstr;
using ::testing::MatchesRegex;
...
  ASSERT_THAT(foo_string, HasSubstr("needle"));
  EXPECT_THAT(bar_string, MatchesRegex("\\w*\\d+"));
```

## Type Assertions
```C++
::testing::StaticAssertTypeEq<T1, T2>();
```
## Assertion Placement
gtest 的断言不一定要放在测试夹具函数中，但是放在普通函数中时，产生 fatal failure 的 Assertion 需要函数的返回值为void，否则会产生编译错误。否则你只能 non-fatal failure 的 Assertion

## Skipping test execution

```C++
TEST(SkipTest, DoesSkip) {
  GTEST_SKIP() << "Skipping single test";
  EXPECT_EQ(0, 1);  // Won't fail; it won't be executed
}
```

## Teaching googletest How to Print Your Values
gtest 判断失败时会参与打印判断的值来帮助调试，它们可以打印基本类型、本地数组、STL容器和支持 `<<` 操作的类型，同时你也可以自己指定打印的信息

```C++
vector<pair<Bar, int> > bar_ints = GetBarIntVector();

// testing::PrintToString 会调用 bar.PrintTo()函数或 << 进行打印输出
EXPECT_TRUE(IsCorrectBarIntVector(bar_ints))
    << "bar_ints = " << testing::PrintToString(bar_ints);
```
## Death Tests
判断程序是否异常终止
```C++
TEST(MyDeathTest, Foo) {
  // This death test uses a compound statement.
  ASSERT_DEATH({
    int n = 5;
    Foo(&n);
  }, "Error on line .* of Foo()");
}
```

# Using Assertions in Sub-routines
## Adding Traces to Assertions
当多个地方调用了同一个函数时,打印函数的调用栈信息

## Propagating Fatal Failures
gtest在出现错误时,只会中断当前的测试函数,而不是整个测试

# Sharing Resources Between Tests in the Same Test Suite
## Global Set-Up and Tear-Down
test fixture 中的 set-up 和 set-down 会在每次执行 TEXT_F 定义的测试时，创建新的 fixture 并调用 set-up 函数，这种情况 set-up 在 test suite 层次执行，同时 也可以让 set-up 在 test program 层次执行。

- you can register multiple environment objects，their SetUp() will be called in the order they are registered，and their TearDown() will be called in the reverse order.but if you use gtest_main and define a global variable to register then the compiler doesn’t guarantee the order in which global variables from different translation units are initialized

## “Catching” Failures
捕获 gtest 产生的 Assertion failure 信息并抛出异常
```C++
// 判断 statement 产生的 message 中是否包含 substring
  EXPECT_FATAL_FAILURE(statement, substring);
  EXPECT_NONFATAL_FAILURE(statement, substring);
// 如果 statement 创建了新线程,捕获所有线程中的消息
  EXPECT_FATAL_FAILURE_ON_ALL_THREADS(statement, substring);
  EXPECT_NONFATAL_FAILURE_ON_ALL_THREADS(statement, substring);
```


## 参数化测试（ Value-Parameterized Tests ）
使用不同的参数测试，而不需要复制多份同样的测试函数
1. 根据不同的 flag 输入进行不同的测试判断
2. 一个接口有不同的实现
3. 测试多个不同的数据输入

## Type-Parameterized Tests
1. 测试逻辑相同的重载接口

## Testing Private Cod
### 测试静态函数
1. 直接 include 待测试的 .cc 文件
2. 将  `private code` 移动到 `foo::internal` 的命名空间，并将 `private declarations` 放入 `*-internal.h`文件，在 . cc 文件和 test 文件中 include `*-internal.h` 文件
### 测试私有成员
1. 申明`test fixture`为待测试类的友员类，并在`fixture`中定义`accessors`存取器，通过存取器访问私有成员，注意： `test fixture` 和待测试类必须在相同的命名空间中，才能成为友员
2. 利用pimpl重构被测代码
```C++
// foo.h
class Foo {
  ...
 private:
  FRIEND_TEST(FooTest, BarReturnsZeroOnNull);

  int Bar(void* x);
};

// foo_test.cc
...
TEST(FooTest, BarReturnsZeroOnNull) {
  Foo foo;
  EXPECT_EQ(foo.Bar(NULL), 0);  // Uses Foo's private member Bar().
}
```


## Registering tests programmatically
```C++
// factory 是一个工厂调用对象或函数指针,用于测试新的测试实例
template <typename Factory>
TestInfo* RegisterTest(const char* test_suite_name, const char* test_name,
                       const char* type_param, const char* value_param,
                       const char* file, int line, Factory factory);
```

## Getting the Current Test’s Name
获取当前正在运行的测试名称
```C++
  // Gets information about the currently running test.
  // Do NOT delete the returned object - it's managed by the UnitTest class.
  const testing::TestInfo* const test_info =
      testing::UnitTest::GetInstance()->current_test_info(); //current_test_info() returns a null pointer if no test is running

  printf("We are in test %s of test suite %s.\n",
         test_info->name(),
         test_info->test_suite_name());
```


## Extending googletest by Handling Test Events
gtest 提供了` event listener API`来接受测试程序的通知和 test failures,可以选择侦听的事件包括测试程序、测试套件或测试的开始和结束 方法等等

# Running Test Programs: Advanced Options
gtest 可以通过环境变量和命令行参数来控制测试函数的行为.可以使用`--help`查看支持的命令行参数.
 your programs must call ::testing::InitGoogleTest() before calling RUN_ALL_TESTS()

## Selecting Tests
### Listing Test Names
`--gtest_list_tests`
### Running a Subset of the Tests
gtest 默认会执行所有定义的测试,如果只想执行部分的测试,需要设置`GTEST_FILTER`环境变量和`--gtest_filter`参数为` filter string`,gtest 只会运行全名称与 filter 匹配的测试.

filter example:
```C++
./foo_test Has no flag, and thus runs all its tests.
./foo_test --gtest_filter=* Also runs everything, due to the single match-everything * value.
./foo_test --gtest_filter=FooTest.* Runs everything in test suite FooTest .
./foo_test --gtest_filter=*Null*:*Constructor* Runs any test whose full name contains either "Null" or "Constructor" .
./foo_test --gtest_filter=-*DeathTest.* Runs all non-death tests.
./foo_test --gtest_filter=FooTest.*-FooTest.Bar Runs everything in test suite FooTest except FooTest.Bar.
./foo_test --gtest_filter=FooTest.*:BarTest.*-FooTest.Bar:BarTest.Foo Runs everything in test suite FooTest except FooTest.Bar and everything in test suite BarTest except BarTest.Foo.
```
### Stop test execution upon first failure
`GTEST_FAIL_FAST` 、`--gtest_fail_fast`

### Temporarily Disabling Tests
如果你有一个暂时无法修复的 `broken test` 你可以使用 `DISABLED_` 排除该测试 

## Repeating the Tests
有时程序的 bug 是偶尔产生，你需要运行多次之后才可能触发，`--gtest_repeat`可以让你执行重复的测试

## Shuffling the Tests
`--gtest_shuffle`可以按照随机的顺序执行测试程序，帮助你揭露测试之间不好的依赖关系

## Distributing Test Functions to Multiple Machines
分发测试程序到多个机器进行并行测试

## Controlling Test Output
`--gtest_brief=1`只打印失败的信息
`--gtest_print_time=0` 不打印测试时间 

### Generating an XML Report

### Generating a JSON Report

## Controlling How Failures Are Reported


# gMock
1. Mock 用于检测代码之间的交互，例如： 对象的哪些函数被调用？调用的顺序？调用了多少次？调用时的参数？返回值？等等
2. 可以直接测试 protected 和 privvate 方法（Mock类不是简单地继承原本的接口）

[gmock使用](https://www.cnblogs.com/aoyihuashao/p/9362003.html)

[gmock分享](https://www.cnblogs.com/welkinwalker/archive/2011/11/29/2267225.html)