#ifndef PALA_APPS_APP_RUNNER_H
#define PALA_APPS_APP_RUNNER_H

void initPalaAPI();
void freeAppExecBuf();
bool loadAndRunApp(const char* path);

#endif // PALA_APPS_APP_RUNNER_H
