/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   mutex.h
 * Author: zxc_work
 *
 * Created on 2020年2月22日, 下午2:08
 */
#pragma once

#ifndef MUTEX_H
#define MUTEX_H
#include <mutex>

extern std::mutex _mutex;

#define CREATE_THREAD_LOCK_GUARD  std::lock_guard<std::mutex> lck(_mutex);

#endif /* MUTEX_H */

