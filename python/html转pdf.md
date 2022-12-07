# 转换工具
## 1. 二进制软件
### wkhtmltopdf
wkhtmltopdf是一个将HTML转换为PDF的开源命令行工具，它是过一个C程序库。wkhtmltoimage则可以将HTNML转换为多种格式的图像

[wkhtmltopdf-github](https://github.com/wkhtmltopdf/wkhtmltopdf)

### PDFKit
PDFKit是对wkhtmltopdf的的封装，使用Ruby脚本和Rails框架实现。其可以将本地HTML和URL转换为PDF，但只支持老版本的HTML+CSS格式。

[pdfkit-github](https://github.com/pdfkit/pdfkit)

python对PDFKit进行了封装，但是其需要在系统中安装wkhtmltopdf工具

pypandoc则可以将HTML转换为Doc

[pdfkit-pypi](https://pypi.org/project/pdfkit/)

## 纯 Python 库实现
### xhtml2pdf
xhtml2pdf 是一个基于 ReportLab、html5lib、PyPDF2 等 Python 模块构建的 HTML 到 PDF 转换模块。能够很好的支持 HTML5 、CSS2.1 和部分 CSS3 语法。

因为是基于 Report Lab 模块进行的开发，其对中文的支持在某些环境下会有问题。

[xhtml2pdf-pypi](https://pypi.org/project/xhtml2pdf/)
[xhtml2pdf-doc](https://xhtml2pdf.readthedocs.io/en/latest/)

### weasyprint
weasyprint 是一个用于 HTML 和 CSS 的可视化渲染引擎，可以将 HTML 文档导出为打印标准的 PDF 文件。

这个模块功能很强大、效果很出色，但是，模块的依赖项太多了

## 浏览器方案
### Web 自动化的浏览器内核
 Selenium、pyppeteer 可以进行数据采集和 Web 自动化测试工作。这两个模块都是用来驱动一个真实的浏览器来进行网页的操作。正是基于此，我们可以调用浏览器中打印相关的 API 接口，来实现 HTML 转 PDF 的功能。

 ### Qt 的 Web 引擎
 在 Qt5 中，Qt 使用新的 Chromium 内核代替了老旧的 WebKit 作为 Web 的渲染引擎。使得在 Qt 中进行可以现代化的浏览器开发。

借助于 Qt 的 Python 实现（PyQt5 系列 和 PySide2 系列），我们可以直接调用 Qt 中的 Web 引擎相关的接口。


## 参考
[python 将html转换为pdf的几种方法](https://www.html.cn/script/python/155860.html)

# 转换方法
## xhtml2pdf
### 安装

### Using with Python standalone
```python
from xhtml2pdf import pisa             # import python module

# Define your data
source_html = "<html><body><p>To PDF or not to PDF</p></body></html>"
output_filename = "test.pdf"

# Utility function
def convert_html_to_pdf(source_html, output_filename):
    # open output file for writing (truncated binary)
    result_file = open(output_filename, "w+b")

    # convert HTML to PDF
    pisa_status = pisa.CreatePDF(
            source_html,                # the HTML to convert
            dest=result_file)           # file handle to recieve result

    # close output file
    result_file.close()                 # close output file

    # return False on success and True on errors
    return pisa_status.err

# Main program
if __name__ == "__main__":
    pisa.showLogging()
    convert_html_to_pdf(source_html, output_filename)
  ```