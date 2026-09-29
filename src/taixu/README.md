# Taixu (太虚)

**太虚（Taixu）** 是一个由 AI 驱动的轻量级沉浸式 3D 世界创作与漫游环境。

它不是一个满屏按钮、用来拉顶点的专业建模工具（如 Blender），也不是体积动辄上百 GB、编译启动极其笨重的商业游戏引擎（如 UE/Unity）。它是一个**秒级打开、以第一/第三人称视角在里面边逛边造、能够由微至巨生长出“庞大且充满细节”的世界、让 AI 现场听指令搭世界的 3D 个人沙盒工作台**。

> **心智模型**：它**有点儿类似 AI 驱动的《我的世界》（Minecraft），但它是完全基于现代真实的 3D 渲染管线与连续几何体，而不是死板的体素/格子块（Voxel Blocks）**。普通人可以像玩建造游戏一样轻松上手，但最终呈现出的是真正高品质的现代 3D 视觉世界。

---

## 一、 远景画像：它到底是个什么形态

想象一个永远轻快、随开随用的 3D 创造沙盒：

1. **第一人称沉浸创作（世界本身即是编辑器）**：
   你不需要脱离世界进入一个传统的 Editor，也不需要像做 CAD 那样在复杂的正交三视图和参数框里点来点去。你直接以人类角色的身位走在三维空间里，**The world itself is the editor**，所见即所造，**Player ≈ Creator**。
2. **AI 是你的现场施工队**：
   你只需看着眼前的场景自然表达意图，AI 负责理解你的空间诉求，实时把几何体、房间、摆件、灯光搭建出来。
3. **随心所欲的美术风貌**：
   你想搭一个写实硬核的地下机房，它就是物理质感的 PBR 光影；你想建一个明朗轻松的动漫小镇，它就是清爽干净的卡通三渲二。引擎原生支持不同风格。
4. **世界的分享与梯级分发（Creation 与 Publication 分离）**：
   遵循 **“Create anything privately; publish under accountability”** 原则。你在私人世界（Private World）中拥有广阔的创作自由；当将空间（如一间酒吧、一个科幻走廊）通过门后拼接或链接分享给朋友、推向社区公开发布时，则依托长效 Creator 身份与声誉逐步解锁更高的分发权限，借助社交图谱实现有机的内容治理。
5. **由微至巨、自然生长的宏大世界（庞大且充满细节）**：
   太虚绝不妥协画质与微观体验，核心特征是**“庞大且充满细节”**。它既能承载连绵数公里、真实城市级的广袤体量，又能容纳近看时每一张海报、每一盏街灯、每一个抽屉内物件的极高细节。
   
   > **核心挑战与终极目标**：
   > 实时流畅渲染这样一个场景，本身在现代图形学中就已经是一项极具挑战的目标（传统 3A 游戏即使面对完全静态死板的资产，也需要上百人团队与数小时的离线烘焙预计算）；
   > 而支持它**从零开始、完全由普通用户在交互中实时自发生长（Organic Growth from Scratch）**，则是一个更高阶的终极目标——没有漫长预计算，世界是全动态、可编辑、随建随逛的活体世界。
   >
   > **空间尺度的终极北极星（大体量不等于慢启动，类似谷歌地球的秒速流式呈现）**：
   > 谷歌地球背后沉淀了整颗行星的庞大数据，却依然能在浏览器或客户端中秒速启动并浏览。**海量数据并不等于启动迟缓**。
   > 太虚追求类似的流式架构哲学：即便背后承载着整座城市乃至更大规模的庞大资产，客户端依然能**秒速冷启动**。通过视锥按需调度、空间分块流式加载与自适应层级，用户可以在 1 秒内踏入米级甚至厘米级的局部细节；也可以在连续的飞行视角下平滑升空拉升（Zoom Out）到街区、城市甚至整颗行星的全貌。不同于谷歌地球纯只读的静态瓦片，太虚视野中的**每一个物件、每一栋建筑，都是活生生、可编辑、由用户与 AI 自发创造出来的实体**。
   >
   > **架构设计备忘（大世界实时流式加载与多级 LOD，现阶段暂不需实现）**：
   > 最终构建的世界可能极其庞大，全部一次性载入内存绝不可能，必须依托一套自适应的实时流式装载机制：
   > * **由近及远、视点优先加载**：加载顺序严格按照相机当前位置由近及远展开。用户启动程序时立刻优先呈现周遭的近景空间，随后在后台按需逐步拉取远处数据。从而让**客户端的启动时间与世界的总规模彻底解耦**；
   > * **物体的多级渐进 LOD（Progressive Multi-tier LOD）**：场景中的物体拥有多个层级（最基础级仅为简单的包围盒占位，过渡级为简易网格与基础材质，高阶为精细模型、高清纹理乃至动画与物理）。加载时从最低级 LOD 先行占位呈现，后台逐步渐进式升级替换为高精度细节，彻底杜绝视野内突兀的跳出卡顿。
6. **AI 即时编程与安全沙箱扩展（AI-Driven Extensibility）**：
   引擎不可能穷尽大千世界中所有特殊的光影与动画奇效。如果用户想要某种特定效果（如黑洞引力透镜、水面焦散、全息故障霓虹、机械升降联动）而引擎暂未内置——凭借现代极其强大的 AI Coding 能力，用户可以直接让 AI **“现场写一个渲染着色器或动画逻辑插件”**，即刻驱动该效果，并作为场景的一部分打包共享给其他人。

   > **隐私、安全与信任边界（Security Sandbox）**：
   > 面对网上随机下载的场景与插件，用户的安全与隐私是不可逾越的底线。太虚**绝不运行任意不受信任的原生二进制代码（如 DLL/so）**，而是构筑了坚固的受限安全沙箱：
   > * **GPU 着色器沙箱**：光影特效纯粹以受校验的着色器形式运行在 GPU 管线上，物理上不具备任何磁盘读写或网络连接能力；
   > * **CPU 逻辑安全沙箱（如 WASM / 受限脚本虚拟机）**：复杂的程序化动画与交互逻辑运行在严格沙箱中，物理隔绝操作系统敏感 API 与外部网络；
   > * **源码透明与自动审查**：插件以透明源代码形态随场景内嵌，引擎与 AI 可以在载入前进行静态安全扫描与死循环校验，确保任何人都可以放心踏入他人分享的奇妙世界。

---

## 二、 平台定位对比：Taixu 与现有体系的根本区别

Taixu 与商业游戏引擎、Roblox、Second Life 和 Minecraft 都存在一定程度的重叠，但目标并不是复制其中任何一种产品。

它们分别代表了几种不同的 3D 内容创建模式：
* **Unreal / Unity**：Developer builds the experience（开发者开发，玩家体验）
* **Roblox**：Creator builds the game（创作者做游戏，玩家玩游戏）
* **Second Life**：Resident builds the virtual world（居民手动建造虚拟世界）
* **Minecraft**：Player builds inside the game（玩家在规则方块网格内建造）
* **Taixu**：**Player describes intent; AI builds and modifies the world**（玩家描述意图，AI 在高品质实时世界中现场建造与演化）

### 1. 商业游戏引擎（Unreal / Unity）
* **传统模式**：`Idea → Editor → Assets → Code/Script → Build → Package → Application → User`。Creator 与 Player 是两个完全隔离的角色。即使引入 AI，也仅是“AI 辅助开发者操作复杂的 Editor”。
* **Taixu 的区别**：**AI is part of the world itself.** 用户不需要成为专业游戏开发者。用户一进入世界，就已经处于创作环境之中（*“在河对面建一个村庄”*、*“给这里增加一座桥”*），没有传统意义上生硬的 `Editor Mode → Build → Run` 边界。**The world itself is the editor.**

### 2. Roblox
* **Roblox 模式**：通过 Roblox Studio 创建 Experience 并发布，极大降低了门槛。但它仍然存在明显的 **Creator / Player 分离**（Studio 是创作端，Client 是消费端）。
* **Taixu 的区别**：**Player ≈ Creator**。任何用户在当前 World 中都可以通过 AI 实时修改或扩展世界。创作（Creation）直接成为漫游与探索（Gameplay / Exploration）本身的一部分。

### 3. Second Life
* **Second Life 模式**：很早就提出了“居民既是玩家也是创造者”，但其根本痛点在于**创作依赖手动 3D 制作技能（Manual 3D authoring skills）**。用户必须学习 Primitive、Transform、Texture、Script 等专业概念，导致真正的创作者依旧是少数专业群体。
* **Taixu 的区别**：**User manipulates meaning and intent.** 用户不需要学习 3D 制作软件。用户说 *“在湖边给我造一个木屋”*，系统自动完成资产挑选、几何生成、摆件布局、材质、光照、碰撞、LOD 与性能优化。

### 4. Minecraft
* **Minecraft 模式**：交互理念与 Taixu 高度一致——**The game itself is the editor**。但 Minecraft 能够做到这一点，是以牺牲真实视觉为代价的：它依赖高度受限的**体素方块世界（Block World）**。
* **Taixu 的区别**：探索一个根本性命题——**如果 AI 可以承担传统 3D 编辑的复杂性，我们是否可以获得 Minecraft 的即时创造体验，而摆脱 Minecraft 的格子限制？**
  * Minecraft：`Simple Representation → Simple Editing`
  * Taixu：`Complex 3D Representation + AI Abstraction → Simple Editing`
  用户依然享有极其轻松直接的创造乐趣，但底层世界是由高精网格、物理、PBR 材质、动态光照与程序化系统组成的真实连续 3D 空间。

### 5. 核心架构差异：语义世界模型（Semantic World Model）
传统引擎主要把世界看作 `Scene Graph + Components + Assets`；Minecraft 把世界看作 `Blocks + Entities`；Roblox 把世界看作 `Instances + Scripts`。

Taixu 引入了更高维度的表述：**Semantic World Model（语义世界模型）**。
* 物体不仅仅是 `Mesh_02341`，更知道自己是 `Wooden Cabin`；
* 并进一步理解空间拓扑与常识关系：木屋有一扇门、木屋包含房间、房间包含家具、木屋隶属于小村庄、村庄依傍着河流、河流连通着湖泊；
* **AI 操作的是这些语义概念与空间关系，而 Renderer 最终看到的是 Mesh、Material 与 Light**。这是 Taixu 与传统游戏引擎之间最根本的架构分野。

### 6. Taixu 的创作循环（The Creation Loop）
* 传统引擎：`Developer → Editor → Code/Assets → Build → Player`
* Roblox：`Creator → Roblox Studio → Publish → Player`
* Minecraft：`Player → Block Interaction → World Changes`
* **Taixu**：`Player → Intent → AI → World Changes → Player`
  随之立即进入新的闭环：
  $$\textbf{Explore} \longrightarrow \textbf{Imagine} \longrightarrow \textbf{Describe} \longrightarrow \textbf{Generate} \longrightarrow \textbf{Explore}$$
  这个沉浸式探索与创造循环本身就是产品的核心体验。

### 7. 平台对比总表

| 维度 | 商业引擎 (Unreal / Unity) | Roblox | Second Life | Minecraft | **Taixu (太虚)** |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **主要创作者** | 专业开发者 | 创作者 | 虚拟居民 | 游戏玩家 | **玩家即创作者 (Player ≈ Creator)** |
| **创作界面** | 专业 Editor + 代码 | Roblox Studio + 脚本 | 世界内手工对象编辑面板 | 直接方块破坏/放置交互 | **自然语言 + 沉浸直觉辅助 (Natural Language + Intuitive Assist)** |
| **创作发生在体验内部** | 否（完全分离） | 大多否（分 Studio 与 Client） | 是 | 是 | **是（The world itself is the editor）** |
| **AI 原生架构** | 否（仅外挂辅助插件） | 否 | 否 | 否 | **是（AI 是世界核心运转协议）** |
| **世界表达体系** | 场景图 / 资产 / 实体组件 | 实例树 / 脚本 | 对象网格 / 脚本 | 体素方块 / 实体 | **语义世界模型 + 运行时渲染管线** |

### 8. 定位合成与终极假设
Taixu 是已有优秀思想的集大成与升华：
* 借鉴 **Game Engines**：高品质、低延迟、现代实时 3D 渲染内核；
* 借鉴 **Roblox**：创作者平台、分发机制与社交生态；
* 借鉴 **Second Life**：用户自由构建、持久共存的世界与社交在场感；
* 借鉴 **Minecraft**：世界即编辑器（The world itself is the editor）；
* 注入 **AI-native 语义世界生成** 作为全新基石。

> **终极检验假设**：  
> **Can AI remove the complexity barrier of creating rich 3D worlds?**  
> （AI 是否能够彻底打破创造丰富 3D 世界的技术门槛？）  
> 用户不再需要学习任何 3D 软件，甚至不需要意识到自己正在使用一个“编辑器”。用户只需：**Explore. Imagine. Describe.**，太虚负责将意图转化为即刻可进入、可交互、可继续修改的真实 3D 世界。

---

## 三、 产品与治理设计原则

AI 会让 3D 内容的生产成本急剧趋近于零。优质内容容易爆发，但垃圾、骚扰、违规内容的成本也同样归零。单纯依靠传统的“生成 $\rightarrow$ 审查 $\rightarrow$ 封禁”模式无法支撑平台的可持续治理。太虚确立以下核心产品与治理原则：

### 1. Creation 与 Publication 严格分离
> **Create anything privately; publish under accountability.**  
> （私下创作拥有广泛自由；公开发布必须承担责任。）

用户在自己的私人世界（Private World）中进行实验与构思，与将内容推向公共空间是两个截然不同的行为层级。避免为了公共空间的合规治理，而过度绑架或扼杀私人创作的自由度。

### 2. Creator Identity 与长期声誉绑定
不仅依靠被动内容审核，而是建立**长期、可追责的创作者身份体系**：
$$\text{Real Identity} \longrightarrow \text{Verified Account} \longrightarrow \text{Persistent Creator Identity} \longrightarrow \text{Public Pseudonym}$$
* **目的并非公开实名**：普通用户看到的依然可以是昵称/代号；
* **核心在于建立长效约束**：持续身份（Persistent Identity）、追责机制（Accountability）、声誉积累（Reputation）、创作历史（Creation History）与发布成本（Publishing Cost）；
* 优秀作品积累长期声誉，恶意滥用与劣质垃圾同样损耗长期身份，借助现实社交契约极大提升恶意发布的社会成本。

### 3. 创作自由 vs 分发责任（Graduated Distribution）
> **Freedom to create is broad. Freedom to distribute is earned.**  
> （创作的自由很广阔，但分发的自由需要凭声誉获取。）

系统不需要在 AI 生成的每一步都过度卡脖子，而是将治理重心收敛在**分发层（Distribution Layer）**：
* **私人创作（Private Creation）**：极少限制，充分挥洒创意；
* **好友共享（Share with Friends）**：开始受到基础内容规范约束；
* **公开发布（Public World）**：需满足公共社区内容标准；
* **发现与推荐（Discovery / Recommendation）**：要求更高的 Creator 声誉与内容质量；
* **大规模全网分发（Large-scale Distribution）**：执行最高级别的安全、品质与身份认证标准。

### 4. 社交图谱（Social Graph）本身就是治理架构
太虚不需要从第一天起就构造一个完全匿名、所有人混杂的大公域。世界围绕：
$$\text{Creator} \longrightarrow \text{Friends} \longrightarrow \text{Followers} \longrightarrow \text{Communities} \longrightarrow \text{Public Discovery}$$
逐渐生长与扩散。社交图谱不仅是社交功能，更是内容治理机制的关键组成部分。

---

## 四、 移动端兼容性：Must Have 架构约束

移动端支持绝不能被视为项目后期的“外围移植特性（Porting Feature）”，而必须作为**第一天就焊死在架构底座中的强硬约束（Architectural Constraint）**：

> **Every Taixu world must be consumable on a modern mobile device. Creation may use more powerful compute, but viewing and interaction cannot require a PC.**  
> （每一个太虚世界都必须能在现代移动设备上流畅消费。创作可以使用更强大的计算力，但漫游与交互绝不能强制依赖 PC。）

### 1. 核心底线：Consumable（可消费、可交互）
手机端初期不一定拥有 PC 端所有重型专业编辑操作，但必须保证：
* 能够秒开进入 World；
* 能够流畅漫游与交互 World；
* 拥有基础的创造能力；
* **能够通过自然语言 Prompt 指挥 AI 修改 World**。

### 2. AI 创作天然契合移动端
传统 3D 软件在手机上无法施展，是因为庞杂的工具栏、属性面板、Gizmo 轴向手柄、材质节点图无法塞进触摸屏。
而在太虚中，创作的核心界面是**自然语言**（*“在这里建一座木屋”、“把森林延伸到山脚”*）。**AI-driven Creation 第一次让手机真正有机会成为可用的 3D 世界创作设备，而不仅仅是只读播放器。**

### 3. 跨设备伸缩性由系统负责（System-Owned Scalability）
创作者绝不需要手工分别制作“PC 版”、“手机版”、“高画质版”、“低画质版”：
> **Creator creates one world; Taixu decides how that world is rendered on each device.**  
> （创作者只需创造一个世界；太虚系统负责决定它在不同设备上如何高效呈现。）

底层运行时根据设备算力与热功耗自动调度：几何 LOD、纹理分辨率、材质着色复杂度、光影品质、阴影开销、物理更新频率、对象密度、视距裁剪与流式步长。

### 4. AI 生成器必须理解运行时预算（Runtime Budget）
移动端性能保障绝不能只由渲染器最后死扛。**AI 在生成世界的那一刻，本身就必须理解目标运行时的性能预算**。
AI 生成协议中天生内嵌性能约束：禁止无节制生成数千万面低价值道具、滥用海量 8K 贴图、制造数千个无意义独立 Draw Call。AI 生成公式始终兼顾：
$$\textbf{Semantic Intent} + \textbf{Visual Quality} + \textbf{Runtime Budget}$$

---

## 五、 UI 设计底线原则：零 3D 门槛，UI 仅作为 AI 输入的辅助

**太虚绝对不要专业 3D 建模软件那种满屏面板、复杂坐标与参数滑块。** 我们的目标用户是对 3D 建模、空间拓扑、着色器没有任何先验知识的普通人。

UI 在太虚中存在的**唯一使命**，是帮助普通用户更轻松地向 AI 表达意图，解决“不知道跟 AI 说什么”和“不知道怎么微调”的问题。

> **说明**：以下列出的交互形态仅作为现阶段的**参考设计与思路示例**，用于具象化阐释设计原则，并不代表最终的固定使用形态。随着产品迭代与实际用户测试，具体的呈现方式会持续演进：

1. **灵感脚手架（Prompt Pills，参考思路）**：
   面对空旷世界，视口弹出大白话灵感按钮（如 *“生成复古书房”*、*“变出阳光小院”*），随手一点即获正反馈，消除空白画布恐惧。
2. **上下文大白话气泡（Contextual Bubbles，参考思路）**：
   点中一张桌子，不弹复杂的数值面板，只冒出几个智能意图选项（*“桌上摆点东西”*、*“换个浅色木质”*、*“换个款式”*、*“删掉”*），或者直接对它说话。
3. **生活常识级直觉交互（Sims 级交互，参考思路）**：
   推动物体时自动吸附地面/桌面，绝不凌空插模；滚轮转方向，贴墙自动靠齐。不需要懂欧拉角和变换矩阵。
4. **像“挑衣服”一样的方案轮播（Variation Carousel，参考思路）**：
   AI 摆放物件后，上方提供简单的 `< 换一个 >` 翻页箭头，像网购挑款式一样直观选择，不满意一键切换。

---

## 六、 典型使用场景与用户体验

### 场景 1：在第一人称世界里“边逛边造”
* **体验**：双击启动，没有资源解压与漫长预热，1 秒内直接置身于三维视口中，按 WASD 和鼠标自由走动。
* **交互**：
  * 走到一面空墙前，看着它：“把这堵墙向后推 3 米，做成一整面嵌入式书架，放满旧书。”
  * 看着地板：“铺上胡桃木地板，并在房间正中央放一张咖啡桌和两把皮椅。”
  * 觉得不对劲：“咖啡桌太大了，缩小三分之一，颜色换成浅灰。”
  * 觉得不满意随时 `Ctrl+Z`，刚才的变化一键完整回滚。

### 场景 2：跨风格的快速搭建与探索
* **体验**：无论是做赛博朋克写实风、二次元卡通风、还是极简 Low-Poly，你不需要为了换画风去重配整套渲染管线。在场景设置里切个风格预设，或者告诉 AI：“把这个房间改成吉卜力动画画风”，墙壁着色、描边轮廓和光照即时响应，始终保持帧率流畅。

### 场景 3：世界共享与“门后拼接”
* **体验**：
  * 你建好了一个带有未来感的空间站走廊，朋友用太虚建了一座赛博拉面馆。
  * 朋友把他的拉面馆世界包（或链接）发给你，你指着走廊尽头的一扇气闸门：“把拉面馆接在这扇门后”。
  * 你走上前推开气闸门，客户端按需快速流式装载门后空间，不需要打断体验的黑屏读条，直接抬脚跨进他的拉面馆，两个人创造的世界就此连通。

### 场景 4：从一间小居酒屋“生长”出一整座城
* **体验**：
  * 你最开始只造了一间温馨的深夜居酒屋。
  * 某一天你觉得单调，推开居酒屋的大门看着外面的虚空：“在门外铺一条雨夜的石板路小巷，两侧开几家杂货铺和拉面摊。”
  * 几周后，朋友们沿着小巷两端继续向外建造公寓楼、地铁入口和中心广场。
  * 几个月过去，最初那一间小居酒屋的门外，已经向外蔓延生长出一整座连绵数公里、充满生活烟火气与丰富细节的巨型城市。你站在摩天大楼的顶层俯瞰整个城市夜景，还能看到最早那家小酒馆依然亮着暖黄色的招牌。

### 场景 5：AI 现场写插件：“来个黑洞引力透镜特效”
* **体验**：
  * 你在太空中建造了一个黑洞空间站，但觉得引擎自带的常规光照表现不出黑洞的壮丽，对 AI 说：“我想让这个黑洞周围的光线像星际穿越那样被强烈弯曲扭曲，但引擎默认没有这个功能。”
  * AI 现场写了一段专属的引力透镜后处理着色器，自动注入视口管线，黑洞周遭的星图瞬间产生强烈的弯曲折射。
  * 你把这个世界打包发给朋友，朋友的客户端自动在一个受保护的 GPU 着色器沙箱中安全运行该插件，朋友立刻体验到了相同的震撼视觉，且完全无需担心恶意程序或木马风险。

### 场景 6：移动端随身漫游与自然语言即时改写
* **体验**：
  * 在 PC 高性能工作站上由 AI 协助精细构筑的宏大世界，你掏出手机同样能秒速进入漫游（Consumable），无需 PC 硬件绑架。
  * 手机端凭借系统统管的动态伸缩性，自动调度适合移动 GPU 算力与温控的 LOD、纹理与视距，依然保持 60 FPS 流畅操作。
  * 你在手机上走到广场中央，不需要复杂的触屏面板，直接对着手机麦克风说：“在喷泉四周加几圈木质长椅与绿植。”
  * AI 充分理解移动端性能预算（Runtime Budget），生成轻量且优雅的摆件结构，场景在眼前即刻更新成型。

---

## 七、 运行环境与需求分级

### 1. 运行平台定位
* **桌面原生客户端（首发与核心主力）**：以桌面端原生应用（Windows / Linux）为主要开发与运行基石，充分利用硬件底层 Vulkan 驱动，实现零延迟 60+ FPS 与秒级冷启动；本地直接畅通对接 AI 工具链（MCP / 本地 Agent）。
* **移动端（Must Have 核心约束）**：所有世界必须能在现代移动设备上流畅消费，支持第一人称漫游并直接用自然语言 Prompt 驱动 AI 创造与修改。
* **网页端（轻量只读分发 $\rightarrow$ 远期全功能探索）**：近期作为只读分发手段（轻量 Web 3D 格式导出或 WebRTC 推流漫游）；全功能网页端作为远期探索目标保留，后续深入调研可行性与性能瓶颈。

### 2. 需求优先级分级管理（Requirement Levels）
为了保证庞大架构稳步推进而不失控，所有特性遵循严格的三级演进管理：

* **第一级：Must Have（强架构约束）**
  *如果后期再加会导致底层架构推倒重来的核心特性，必须从第一天就纳入底层架构设计*：
  * Mobile Compatibility（移动端可消费性）
  * Runtime Scalability（设备性能自适应伸缩）
  * World Streaming（由近及远、大世界动态流式加载）
  * AI-editable Scene Representation（AI 原生可编辑的场景语义表述）
* **第二级：Core Product Principle（核心产品原则）**
  *决定太虚是什么、以及为什么根本不同于传统游戏引擎和 UGC 平台的核心基石*：
  * The world itself is the editor（世界本身即编辑器）
  * AI-native Creation（AI 原生驱动，非外挂插件）
  * Creation / Publication Separation（创作与发布权责分离）
  * Persistent Creator Identity & Reputation（长效创作者身份与声誉体系）
  * Reputation-based Distribution（基于声誉的梯级分发）
  * Social Graph as Governance（社交图谱作为内容治理基石）
* **第三级：Later Feature（后期渐进特性）**
  *不影响底层核心架构、可以随着产品演进逐步扩充的业务功能*：
  * 丰富的社交动作与角色表情、复杂的物理玩具交互、商业化道具结算等。

---

## 八、 非目标（Non-Goals：太虚明确不想做什么）

为了保持轻量、极速与纯粹，太虚划定了清晰的边界，明确以下方向**不属于**本项目的目标：

1. **不做重型、全功能的传统 DCC 建模软件（Not a Blender/Maya）**：
   * 不做复杂的顶点拓扑细分（Subdivision）、手动拉点布线、三维雕刻（Sculpting）、UV 展开与骨骼蒙皮权重绘制。
   * 那些是专业数字艺术家的工作。太虚聚焦于“普通人凭生活直觉和 AI 语言搭建、组装与延展世界”，而非专业网格资产的从零手工塑形。

2. **不做臃肿复杂的全管线商业游戏开发引擎（Not a UE/Unity）**：
   * 不做庞大的游戏逻辑行为树、复杂的多状态动画蓝图、大型物理刚体布娃娃破碎、繁复的节点式材质图编辑器，以及面向全平台的商业游戏打包发布系统。
   * 太虚是**轻量级、秒开、自下而上生长的 3D 沉浸式世界沙盒与创作者空间**，绝不承担通用商业游戏开发流水线的历史包袱。

3. **不做 2D、俯视角或 RTS 视界（No 2D, Isometric, or RTS Viewports）**：
   * 不兼容 45 度等轴俯视角（Isometric）、不兼容类似红警/星际的 RTS 上帝排版视角、不做 2D 横向卷轴平台跳跃。纯粹死磕**第一人称与第三人称的 3D 真实身位沉浸**。

4. **不做黑盒式、不可控的纯端到端神经渲染器（Not a Pure Neural/Splat Black-Box）**：
   * 绝不以高斯泼溅（Gaussian Splatting）或纯神经隐式场（NeRF）作为世界的主体表述。
   * 绝不妥协世界的确定性、拓扑可编辑性、碰撞物理体与可持久化物体身份。

5. **不做封闭、垄断的专有格式花园（Not a Walled Garden）**：
   * 不采用私有加密的二进制死格式；
   * 不与任何单一 AI 厂商死绑定（通过开放的 MCP/RPC 标准协议对接任何通用大模型）；
   * 世界的数据与指令完全透明开源、Git 友好，支持创作者自由导出、迁移与二次分发。

6. **现阶段不优先强求纯网页端重型创作（Not a Web-First Heavy IDE at Current Stage）**：
   * 在当前攻坚阶段，绝不为了迁就浏览器的受限环境而妥协核心引擎的渲染特性与底层性能。桌面原生客户端是攻坚阶段的主阵地；全功能网页端作为远期目标保留调研，不占用早期的核心研发资源。
