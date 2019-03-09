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

const rootHostContext = {};
const childHostContext = {};

const JUCERenderer = Reconciler({
  now: Date.now,

  getRootHostContext: () => {
    return rootHostContext;
  },

  prepareForCommit: () => {},

  resetAfterCommit: () => {},

  getChildHostContext: () => {
    return childHostContext;
  },

  shouldSetTextContent: (type, props) => {
    return false;
  },

  createInstance: (type, newProps, rootContainerInstance, _currentHostContext, workInProgress) => {
    const yueElement = gui.Container.create();
    Object.keys(newProps).forEach(propName => {
      const propValue = newProps[propName];
      if (propName === 'children') {
        if (type !== 'text') {
          if (typeof propValue === 'string' || typeof propValue === 'number') {
            throw new Error('Text strings must be rendered within a <Text> component.');
          }

          if (propValue instanceof Array) {
            propValue.forEach(item => {
              if (typeof item === 'string') {
                throw new Error('Text strings must be rendered within a <Text> component.');
              }
            });
          }
        }
      } else if (propName === 'style') {
        yueElement.setStyle(propValue);
      }
    });
    return yueElement;
  },

  createTextInstance: (text, rootContainerInstance, hostContext, internalInstanceHandle) => {
    return gui.Label.create(text);
  },

  appendInitialChild: (parent, child) => {
    parent.addChildView(child);
  },

  appendChild(parent, child) {
    parent.addChildView(child);
  },

  finalizeInitialChildren: (yueElement, type, props) => {},

  supportsMutation: true,

  appendChildToContainer: (parent, child) => {
    parent.setContentView(child);
  },

  prepareUpdate(yueElement, oldProps, newProps) {
    return true;
  },

  commitUpdate(yueElement, updatePayload, type, oldProps, newProps) {},

  commitTextUpdate(textInstance, oldText, newText) {},

  removeChild(parentInstance, child) {}
});

export function render(element, container, callback) {
  // if (!container._rootContainer) {
  //   container._rootContainer = JUCERenderer.createContainer(container, false);
  // }

  return JUCERenderer.updateContainer(element, container, null, callback);
}
