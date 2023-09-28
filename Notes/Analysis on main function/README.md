# Main() Function 分析
## Main()調用分析
在 Linux 核心設計課程中讀到下面這段

在[C99規格書](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf)中 5.1.2.2.1 內有提到 C Standard 要求 main 函數必須這樣寫：

```c=
int main(void) {...}
```

或是如下：
```c=
int main(int argc, char* argv[]) {...}
```

進而好奇，main背後的機制，和為何C語言中程式一定都從main()開始和結束，進而讀到下面這篇文章：[C程序調用原理](https://www.zhihu.com/question/338814178/answer/783725339)

文章中做了下面的實驗來解說到底在C語言中main()是如何開始和結束的。

以下面的code為例：

```c=
int main(int argc, char* argv[]) {
    return 0;
}
```

一般C語言在*.c變成可執行檔時，會經過下面幾個編譯過程：
![](https://i.imgur.com/Zwl7imt.png)

而在編譯過程時，我們會使用：

```bash=
gcc -o test test.c
```

然而這邊其實是忽略了一個默認的參數" lc" ，也就是默認連接libc函數，故而上面的編譯過程相當於：

```bash=
gcc -o test test.c -lc
```

可以先用下面方式來看看未經libc連結的object file長怎樣：
這邊會用到查看二進位檔案資訊的linux tool為objdump，[教學連結在此](https://jasonblog.github.io/note/linux_tools/er_jin_wei_shi_yong_cheng_5f0f28_objdump_,_readelf.html)：

```bash=
$ gcc -c -o test.o test.c
$ objdump -dS test.o

test_org.o      檔案格式 elf64-x86-64


.text 區段的反組譯：

0000000000000000 <main>:
   0:	f3 0f 1e fa          	endbr64 
   4:	55                   	push   %rbp
   5:	48 89 e5             	mov    %rsp,%rbp
   8:	89 7d fc             	mov    %edi,-0x4(%rbp)
   b:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
   f:	b8 00 00 00 00       	mov    $0x0,%eax
  14:	5d                   	pop    %rbp
  15:	c3                   	retq
```

可以很明顯看出就是一個main函數，再來看看經libc連結過後會長怎樣：

```bash=
$ ld -o mytest -lc mytest.o
ld: warning: cannot find entry symbol _start; defaulting to 000000000040020b
```
可以從警告訊息得知目前還沒有start point，其實除了連結libc以外，complier在編譯的過程其實還連結了一些其他的必要object file，這些object file的具體位置依照系統不同而不同

連結方式如下：

```bash=
ld /usr/lib/x86_64-linux-gnu/crt1.o /usr/lib/x86_64-linux-gnu/crti.o /usr/lib/x86_64-linux-gnu/crtn.o -o test -lc test.o /lib64/ld-linux-x86-64.so.2
```
可以看出，還會用到crt1.o, crti.o, crtn.o
經過連結後，再來看看最終可執行檔和未連結的區別

```bash=
$ objdump -dS test

test      檔案格式 elf64-x86-64


.init 區段的反組譯：

0000000000401000 <_init>:
  401000:	f3 0f 1e fa          	endbr64 
  401004:	48 83 ec 08          	sub    $0x8,%rsp
  401008:	48 8b 05 e9 2f 00 00 	mov    0x2fe9(%rip),%rax        # 403ff8 <__gmon_start__>
  40100f:	48 85 c0             	test   %rax,%rax
  401012:	74 02                	je     401016 <_init+0x16>
  401014:	ff d0                	callq  *%rax
  401016:	48 83 c4 08          	add    $0x8,%rsp
  40101a:	c3                   	retq   

.text 區段的反組譯：

0000000000401020 <_start>:
  401020:	f3 0f 1e fa          	endbr64 
  401024:	31 ed                	xor    %ebp,%ebp
  401026:	49 89 d1             	mov    %rdx,%r9
  401029:	5e                   	pop    %rsi
  40102a:	48 89 e2             	mov    %rsp,%rdx
  40102d:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
  401031:	50                   	push   %rax
  401032:	54                   	push   %rsp
  401033:	49 c7 c0 d0 10 40 00 	mov    $0x4010d0,%r8
  40103a:	48 c7 c1 60 10 40 00 	mov    $0x401060,%rcx
  401041:	48 c7 c7 d5 10 40 00 	mov    $0x4010d5,%rdi
  401048:	ff 15 a2 2f 00 00    	callq  *0x2fa2(%rip)        # 403ff0 <__libc_start_main@GLIBC_2.2.5>
  40104e:	f4                   	hlt    
  40104f:	90                   	nop

0000000000401050 <_dl_relocate_static_pie>:
  401050:	f3 0f 1e fa          	endbr64 
  401054:	c3                   	retq   
  401055:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  40105c:	00 00 00 
  40105f:	90                   	nop

0000000000401060 <__libc_csu_init>:
  401060:	f3 0f 1e fa          	endbr64 
  401064:	41 57                	push   %r15
  401066:	4c 8d 3d 13 2e 00 00 	lea    0x2e13(%rip),%r15        # 403e80 <_DYNAMIC>
  40106d:	41 56                	push   %r14
  40106f:	49 89 d6             	mov    %rdx,%r14
  401072:	41 55                	push   %r13
  401074:	49 89 f5             	mov    %rsi,%r13
  401077:	41 54                	push   %r12
  401079:	41 89 fc             	mov    %edi,%r12d
  40107c:	55                   	push   %rbp
  40107d:	48 8d 2d fc 2d 00 00 	lea    0x2dfc(%rip),%rbp        # 403e80 <_DYNAMIC>
  401084:	53                   	push   %rbx
  401085:	4c 29 fd             	sub    %r15,%rbp
  401088:	48 83 ec 08          	sub    $0x8,%rsp
  40108c:	e8 6f ff ff ff       	callq  401000 <_init>
  401091:	48 c1 fd 03          	sar    $0x3,%rbp
  401095:	74 1f                	je     4010b6 <__libc_csu_init+0x56>
  401097:	31 db                	xor    %ebx,%ebx
  401099:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
  4010a0:	4c 89 f2             	mov    %r14,%rdx
  4010a3:	4c 89 ee             	mov    %r13,%rsi
  4010a6:	44 89 e7             	mov    %r12d,%edi
  4010a9:	41 ff 14 df          	callq  *(%r15,%rbx,8)
  4010ad:	48 83 c3 01          	add    $0x1,%rbx
  4010b1:	48 39 dd             	cmp    %rbx,%rbp
  4010b4:	75 ea                	jne    4010a0 <__libc_csu_init+0x40>
  4010b6:	48 83 c4 08          	add    $0x8,%rsp
  4010ba:	5b                   	pop    %rbx
  4010bb:	5d                   	pop    %rbp
  4010bc:	41 5c                	pop    %r12
  4010be:	41 5d                	pop    %r13
  4010c0:	41 5e                	pop    %r14
  4010c2:	41 5f                	pop    %r15
  4010c4:	c3                   	retq   
  4010c5:	66 66 2e 0f 1f 84 00 	data16 nopw %cs:0x0(%rax,%rax,1)
  4010cc:	00 00 00 00 

00000000004010d0 <__libc_csu_fini>:
  4010d0:	f3 0f 1e fa          	endbr64 
  4010d4:	c3                   	retq   

00000000004010d5 <main>:
  4010d5:	f3 0f 1e fa          	endbr64 
  4010d9:	55                   	push   %rbp
  4010da:	48 89 e5             	mov    %rsp,%rbp
  4010dd:	89 7d fc             	mov    %edi,-0x4(%rbp)
  4010e0:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  4010e4:	b8 00 00 00 00       	mov    $0x0,%eax
  4010e9:	5d                   	pop    %rbp
  4010ea:	c3                   	retq   

.fini 區段的反組譯：

00000000004010ec <_fini>:
  4010ec:	f3 0f 1e fa          	endbr64 
  4010f0:	48 83 ec 08          	sub    $0x8,%rsp
  4010f4:	48 83 c4 08          	add    $0x8,%rsp
  4010f8:	c3                   	retq   
```

可以再使用下面指令得到程式的entry point：

```bash=
ELF 檔頭：
  魔術位元組：   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00 
  類別:                              ELF64
  資料:                              2 的補數，小尾序(little endian)
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI 版本:                          0
  類型:                              EXEC (可執行檔案)
  系統架構:                          Advanced Micro Devices X86-64
  版本:                              0x1
  進入點位址：               0x401020
  程式標頭起點：          64 (檔案內之位元組)
  區段標頭起點：          13808 (檔案內之位元組)
  旗標：             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         12
  Size of section headers:           64 (bytes)
  Number of section headers:         24
  Section header string table index: 23

```

經由觀察發現，程式起始位置會變為<_start>，這就是程式的正確入口，經過初始化後，會呼叫<__libc_start_main@GLIBC_2.2.5>
並傳遞位址0x4010d0, 0x401060, 0x4010d5作為參數，可以發現這三個位址分別對應至

```bash=
0000000000401060 <__libc_csu_init>:
...
00000000004010d0 <__libc_csu_fini>:
...
00000000004010d5 <main>:
```

即此三個位址的起始入口，又<__libc_csu_init>會呼叫<__init>，而<__libc_csu_fini>會呼叫<__fini>

經由上面的分析，就能清楚的知道，在連結時，我們可以從crt1.o, crti.o, crtn.o得到一些必要的<__init>, <__fini>和<__start>
並從libc得到<__libc_csu_init>和__libc_csu_fini>，在<__libc_start_main@GLIBC_2.2.5>裡執行一些執行main前的操作，才去執行main函數
而main函數返回後，還得執行回收操作<__libc_csu_fini>

上述就是隱藏在編譯器編譯成可執行檔背後的故事。

因此，了解程式是一連串的調用行為後，就能合理理解規範的重要性和return值的重要。

規格書中有提到下面一段文字：

>If the return type of the main function is a type compatible with int, a return from the initial call to the main function is equivalent to calling the exit function with the value returned by the main function as its argument; reaching the } that terminates the main function returns a value of 0. 
If the return type is not compatible with int, the termination status returned to the host environment is unspecified.

因此，未來要盡量避免 void main()的使用

## 查看二進制檔案的資訊
Linux上有很多的tool可以查看二進制檔案的資訊，包括了objdump、readelf、addr2line、strip、ar、nm、ldd、ngprof、gcov等等。
### objdump - 顯示二進位檔案資訊
objdump可以用根據object file來生成可讀性較佳的彙編檔，使用方式如下所示：

```bash=
$ gcc -c -o test.o test.c
$ objdump -dS test.o
```
而詳細的命令內容，可以用如下方式得到

```bash=
$ man objdump
```
### readelf - 顯示elf檔案資訊
Linux平台下主要有三種可執行檔：

1. a.out（assembler and link editor output 彙編器和連結編輯器的輸出）
2. COFF（Common Object File Format 通用物件檔案格式）
3. ELF（Executable and Linking Format 可執行和連結格式）
#### a.out執行檔格式和內容
而a.out則包含了7個section，格式如下：

```bash=
exec header（執行頭部，也可理解為檔頭部）
text segment（文本段）
data segment(資料段)
text relocations(文本重定位段)
data relocations（資料重定位段）
symbol table（符號表）
string table（字串表）
```
執行頭部的資料結構：

```bash=
struct exec {
    unsigned long a_midmag; /* 魔數和其它資訊 */
    unsigned long a_text; /* 文本段的長度 */
    unsigned long a_data; /* 資料段的長度 */
    unsigned long a_bss; /* BSS段的長度 */
    unsigned long a_syms; /* 符號表的長度 */
    unsigned long a_entry; /* 程式進入點 */
    unsigned long a_trsize; /* 文本重定位表的長度 */
    unsigned long a_drsize; /* 資料重定位表的長度 */
};
```
可以看出，a.out的格式十分緊湊，只會包含有程式運行必要的資訊，而每個section的順序是固定的。

這種結構沒有擴展性，而a.out檔中包含符號表和兩個重定位表，這三個表的內容在連結object file，用以生成可執行檔。
而最終可執行檔的a.out文件中，這三個表的長度都為0。
#### ELF檔格式與內容
Executable and linking format(ELF)檔是Linux系統下的一種常用目的檔案(object file)格式，有三種主要類型:

    (1)適於連接的可重定位文件(relocatable file)，可與其它目的檔案一起創建可執行檔和共用目的檔案 （.obj or .o）

    (2)適於執行的可執行檔(executable file)，用於提供程式的進程映射，載入的記憶體執行。

    (3)共用目的檔案(shared object file),連接器可將它與其它可重定位文件和共用目的檔案連接成其它的目的檔案，動態連接器又可將它與可執行檔和其它共用目的檔案結合起來創建一個進 程映射。

ELF檔可以從兩個角度來檢視，一是從程式連結的角度，二是從程式運行的角度。

ELF header會在檔案開始處描述整個檔案的架構
* Section：提供了object file的各種資訊（如指令、資料、符號表、重定位表等等）
* Program header table：含有每個program header的入口
* Section header table：包含每一個section的入口，給出名字、大小等資訊

下面是ELF header的資料結構：

```bash=
typedef struct {
    unsigned char e_ident[EI_NIDENT]; /* 魔數和相關資訊 */
    Elf32_Half e_type; /* 目的檔案類型 */
    Elf32_Half e_machine; /* 硬體體系 */
    Elf32_Word e_version; /* 目的檔案版本 */
    Elf32_Addr e_entry; /* 程式進入點 */
    Elf32_Off e_phoff; /* 程式頭部偏移量 */
    Elf32_Off e_shoff; /* 節頭部偏移量 */
    Elf32_Word e_flags; /* 處理器特定標誌 */
    Elf32_Half e_ehsize; /* ELF頭部長度 */
    Elf32_Half e_phentsize; /* 程式頭部中一個條目的長度 */
    Elf32_Half e_phnum; /* 程式頭部條目個數 */
    Elf32_Half e_shentsize; /* 節頭部中一個條目的長度 */
    Elf32_Half e_shnum; /* 節頭部條目個數 */ Elf32_Half
    e_shstrndx; /* 節頭部字元表索引 */

} Elf32_Ehdr;
```
#### readelf指令
readelf指令可以用作分析編譯器是如何創建二進位檔案的。
下面為一示例：

```bash=
$ readelf -h a.out

ELF 檔頭：
  魔術位元組：   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00 
  類別:                              ELF64
  資料:                              2 的補數，小尾序(little endian)
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI 版本:                          0
  類型:                              REL (可重定位檔案)
  系統架構:                          Advanced Micro Devices X86-64
  版本:                              0x1
  進入點位址：               0x0
  程式標頭起點：          0 (檔案內之位元組)
  區段標頭起點：          608 (檔案內之位元組)
  旗標：             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           0 (bytes)
  Number of program headers:         0
  Size of section headers:           64 (bytes)
  Number of section headers:         12
  Section header string table index: 11

```
### Address2line - 將位址對應到相應的檔案和行數
給定一可執行檔的address或是一relocatable object在一個section中的offset
假設 xxx 可以是執行檔或so檔，利用gdb可以找到掛掉的addr資訊，再利用addr2line即可查找到此address對應到的程式碼行數，指令如下所示：

```bash=
$ addr2line -Cfie <xxx/xxx.so> <addr>
```

下面為一範例示範如何從segmentation fault來查看程式是掛在哪一行

```bash=
// g++ cpp-crash.cpp -o a.out -g
#include <stdio.h>

void myprint(int* ptr) {
    printf("%d\n", *ptr);
}

int main() {
    int *ptr = NULL;
    myprint(ptr);
    return 0;
}
```
可以從上面的範例看到程式要從指向NULL的指標，印出該指標指向的值
要注意在編譯時要用-g參數來讓它編譯出有除錯的資訊
再來利用dmesg指令來得到程式出錯的stack位址
如下所示：

```bash=
$ dmesg | grep "a.out"

[22210.543125] a.out[30880]: segfault at 0 ip 000055f18823015d sp 00007ffc2f5b13d0 error 4 in a.out[55f188230000+1000]
```
而這些資訊的說明如下：

```bash=
ip:指令指標暫存器，欄位後面的數字為test程式出錯時程式的執行位址
sp:堆疊指標暫存器
error：錯誤碼，由三個字位組的，從高到低為bit 2 ~ bit 0
bit2：值為1代表user program記憶體存取越界，值為0代表kernel program記憶體存取越界
bit1：值為1代表寫入記憶體存取越界，值為0代表讀出記憶體存取越界
bit0：值為1代表權限不足存取，值為0代表無效地址
```

因此可以從上面得知ip為000055f18823015d，且錯誤為4轉成二進制為100，代表user program試圖讀一個無效地址的內容

再來利用address2line來查找位址對應的行數
```bash=
$addr2line -Cfie ./a.out 000055f18823015d
```