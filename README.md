本个项目实现了基于OLC算法的DNA生长测序的实现。
随机生成一个长度为2000个碱基的DNA序列（ATCG字符串，DNA序列实际上是环状的），然后从这个DNA序列中随机截取长度为150个碱基（取名叫reads）的片段，（会产生好多这样长度为150的片段，并且所有片段要包含原2000个碱基的所有点）
对产生的reads进行去重，然后从里面随机挑选一个，开始overlap拼接生长，选择一个reads，在剩余所有的reads中搜索，发现在剩余的所有reads中某个reads的头部的145个碱基与当前选择的reads的尾部145个碱基完全相同，那么就认为这两个reads之前大概率是连在一块的，那么原来的150个碱基的序列现在就变成了155，也就是我们测量出了5个碱基（目前生物上面的技术无法测量很长的DNA序列，可以测量较短的序列）通过这种不断overlap的方式，应该会生长出完整的2000个碱基的序列吧
在project1中实现了线状DNA的测序和输出
在优化版中进行了优化，用户可以自行输入DNA长度，reads段长度和overlap函数中可拼接段的长度，同时对于原来只能实现线状DNA测序就行优化，可以测试环状DNA，实际的DNA就是环状的。
对于内存空间管理也进行优化，及时释放不使用了的堆区空间。
