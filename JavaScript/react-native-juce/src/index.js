import React, {Component} from 'react';
import once from 'lodash/once';

export function View(props) {
  return props.children;
}

export function Text(props) {
  return props.children;
}

class NativeModuleRegistry {
  constructor() {
    this.modules = {};
  }

  register(name, fn) {
    this.modules[name] = once(fn);
  }

  require(name) {
    return this.modules[name]();
  }
}

global.modules = new NativeModuleRegistry();
export const modules = global.modules;

