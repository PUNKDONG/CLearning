#include <stdio.h>

#include "args.h"
#include "error.h"
#include "grep.h"
#include "logger.h"

int main(int argc, char **argv) {
    CliOptions opts;
    GrepStats stats;
    ErrorCode err = ERR_OK;
    const char *arg_err_msg = "";

    /* TODO(step 1): 初始化日志模块（失败时可降级，仅输出 stderr） */

    /* TODO(step 2): 解析命令行参数；失败时打印 usage 并返回错误码 */
    if (parse_args(argc, argv, &opts, &err, &arg_err_msg) != 0) {
        (void)arg_err_msg;
        print_usage(argv[0]);
        return err;
    }

    /* TODO(step 3): 调用 run_grep，记录/输出执行结果 */
    if (run_grep(&opts, &stats, &err) != 0) {
        return err;
    }

    /* TODO(step 4): 正常收尾（关闭日志） */
    return ERR_OK;
}
