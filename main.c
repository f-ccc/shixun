#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "business.h"

void benchmark_performance(SystemContainer* sys, int scale) {
    char bench_file[40];
    sprintf(bench_file, "bench_%d.csv", scale);
    generate_mock_data_file(bench_file, scale);
    
    SystemContainer temp_sys;
    sys_init(&temp_sys);
    
    printf("\n=== 基准性能压力测试测试报告（规模: %d 条） ===\n", scale);
    clock_t start = clock();
    sys_load_csv(&temp_sys, bench_file);
    clock_t end = clock();
    double duration = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;
    printf("  [数据装载落盘耗时]: %.2f ms\n", duration);
    
    char test_sid[] = "202401000001";
    char test_cid[] = "CS300102";
    
    start = clock();
    int i;
    for(i=0; i<10000; i++) hash_search(&temp_sys.hash_table, test_sid, test_cid);
    end = clock();
    printf("  [哈希表索引精确查找1万次耗时]: %.2f ms\n", ((double)(end - start)/CLOCKS_PER_SEC)*1000.0);
    
    start = clock();
    for(i=0; i<10000; i++) avl_search(temp_sys.avl_root, test_sid, test_cid);
    end = clock();
    printf("  [AVL树动态拓扑检索1万次耗时]:   %.2f ms\n", ((double)(end - start)/CLOCKS_PER_SEC)*1000.0);
    
    sys_destroy(&temp_sys);
}

int main() {
    SystemContainer sys;
    sys_init(&sys);
    
    int choice = 0;
    printf("==================================================\n");
    printf("   欢迎进入校园选课记录检索与大数据分析系统 (2026)  \n");
    printf("==================================================\n");
    
    while (1) {
        printf("\n======= 核心功能操作菜单 =======\n");
        printf("1. 批量自动生成测试数据集 (100-10000条)\n");
        printf("2. 加载外部选课日志文件 (CSV持久化)\n");
        printf("3. 动态插入单条新选课日志 (新增)\n");
        printf("4. 依据主键同步清除选课日志 (删除)\n");
        printf("5. 依据主键高能修改对应成绩 (修改)\n");
        printf("6. 自定义多维精确/模糊检索 (查找)\n");
        printf("7. 执行复合多条件数据筛选与文件导出\n");
        printf("8. 多关键字自定义优先级组合排序\n");
        printf("9. 大数据教务统计分析 (成绩段/热度)\n");
        printf("10. 过期旧选课日志自动批量同步清理\n");
        printf("11. 运行三结构工业级性能对比基准测试\n");
        printf("0. 安全退出系统（数据自动刷盘落盘）\n");
        printf("请输入指令编号: ");
        
        if (scanf("%d", &choice) == 0) {
            printf(">> 输入非法，请重新输入数字指令！\n");
            char clear_buf; while((clear_buf = getchar()) != '\n' && clear_buf != EOF);
            continue;
        }
        
        if (choice == 0) {
            sys_save_csv(&sys, "data_auto_saved.csv");
            sys_destroy(&sys);
            printf(">> 数据已安全落盘保存。程序正常退出，祝同学期末答辩斩获优秀！\n");
            break;
        }
        
        switch (choice) {
            case 1:
                generate_mock_data_file("selection_records.csv", 1000);
                printf(">> 成功在项目根目录下生成包含 1000 条选课规范日志的 selection_records.csv 文件！\n");
                break;
            case 2: {
                int count = sys_load_csv(&sys, "selection_records.csv");
                if(count > 0) printf(">> 成功流式加载 %d 条记录至双向链表、AVL树与链地址哈希表中！\n", count);
                else printf(">> 未找到默认数据文件或加载失败，请先执行选项1生成数据。\n");
                break;
            }
            case 3: { // 增加
                CourseSelection data;
                memset(&data, 0, sizeof(CourseSelection));
                printf("请输入学号(12位): "); scanf("%s", data.student_id);
                printf("请输入姓名: "); scanf("%s", data.name);
                printf("请输入学院: "); scanf("%s", data.college);
                printf("请输入课程编号(8位): "); scanf("%s", data.course_id);
                printf("请输入课程名称: "); scanf("%s", data.course_name);
                printf("请输入学分: "); scanf("%f", &data.credit);
                printf("请输入学期(如2024-02): "); scanf("%s", data.semester);
                printf("请输入选课日期(YYYY-MM-DD): "); scanf("%s", data.course_date);
                printf("请输入成绩(0-100): "); scanf("%d", &data.score);
                
                if (sys_insert_sync(&sys, data)) printf(">> [强同步插入成功] 全索引联动 size+1！\n");
                else printf(">> [错误] 键值重复(该生已修读此课程)，拒绝录入。\n");
                break;
            }
            case 4: { // 删除
                char sid[13], cid[9];
                printf("请输入要删除记录的学生学号: "); scanf("%s", sid);
                printf("请输入要删除记录的课程编号: "); scanf("%s", cid);
                if (sys_delete_sync(&sys, sid, cid)) printf(">> [强同步删除成功] 成功从链表、树、哈希表中彻底清除！\n");
                else printf(">> [未找到目标] 删除失败，请检查主键参数。\n");
                break;
            }
            case 5: { // 修改
                char sid[13], cid[9]; int n_score;
                printf("请输入要修改的学生学号: "); scanf("%s", sid);
                printf("请输入对应课程编号: "); scanf("%s", cid);
                printf("请输入更新后的百分制成绩: "); scanf("%d", &n_score);
                if (n_score < 0 || n_score > 100) { printf(">> 警告：成绩超出法定0-100区间，拦截修改。\n"); break; }
                
                if (sys_update_sync(&sys, sid, cid, n_score)) printf(">> [强同步更新成功] 三索引底层属性已完美同步！\n");
                else printf(">> 修改失败，未命中任何对应的选课记录。\n");
                break;
            }
            case 6: { // 查找
                int stype; char kw[100];
                printf("请选择检索模式 (1.按学号精确 / 2.按姓名模糊 / 3.按课程名模糊): "); scanf("%d", &stype);
                printf("请输入检索关键字: "); scanf("%s", kw);
                sys_search_flexible(&sys, stype, kw);
                break;
            }
            case 7: {
                FilterCriteria criteria;
                memset(&criteria, 0, sizeof(FilterCriteria));
                printf("请输入需过滤的特定课程名称关键字 (输入'none'代表跳过): "); scanf("%s", criteria.course_name);
                if (strcmp(criteria.course_name, "none") == 0) strcpy(criteria.course_name, "");
                criteria.score_min = 0; criteria.score_max = 100;
                execute_filter(&sys, criteria, "filter_output.csv");
                break;
            }
            case 8: {
                SortConfig config;
                config.criterion_count = 2;
                config.key_types[0] = 1;  
                config.directions[0] = -1; 
                config.key_types[1] = 2;  
                config.directions[1] = 1;  
                execute_multi_sort(&sys, config);
                break;
            }
            case 9:
                stats_score_sections(&sys);
                stats_course_capacity(&sys);
                break;
            case 10:
                purge_expired_records(&sys);
                break;
            case 11:
                benchmark_performance(&sys, 100);
                benchmark_performance(&sys, 1000);
                benchmark_performance(&sys, 10000);
                break;
            default:
                printf(">> 该数字指令暂未激活，请重新选择菜单功能。\n");
        }
    }
    return 0;
}
