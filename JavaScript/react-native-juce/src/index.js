import React, {Component} from 'react';
import Reconciler from 'react-reconciler';
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

const JUCERenderer = Reconciler({
});

export function render(element, container, callback) {
  if (!container._rootContainer) {
    container._rootContainer = JUCERenderer.createContainer(container, false);
  }

  return JUCERenderer.updateContainer(element, container._rootContainer, null, callback);
}
