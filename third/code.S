<add>:
push   %ebp
mov    %esp,%ebp
sub    $0x10,%esp
movl   $0x3,-0x4(%ebp)
mov    -0x4(%ebp),%edx
mov    0x8(%ebp),%eax
add    %eax,%edx
mov    0xc(%ebp),%eax
add    %edx,%eax
mov    %eax,-0x4(%ebp)
mov    -0x4(%ebp),%eax
leave  
ret    

<main>:
lea    0x4(%esp),%ecx
and    $0xfffffff0,%esp
pushl  -0x4(%ecx)
push   %ebp
mov    %esp,%ebp
push   %ecx
sub    $0x14,%esp
movl   $0x1,-0x14(%ebp)
movl   $0x2,-0x10(%ebp)
pushl  -0x10(%ebp)
pushl  -0x14(%ebp)
call   804840b <add>
add    $0x8,%esp
mov    %eax,-0xc(%ebp)