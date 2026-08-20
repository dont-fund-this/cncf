use crate::impl_mod::COUNT as BUILD;
use crate::r#type::{Def, Defs, Sid, Tag};
use core::ffi::c_void;

extern "C" {
    fn malloc(size: usize) -> *mut c_void;
    fn realloc(ptr: *mut c_void, size: usize) -> *mut c_void;
}

pub struct DefList {
    items: *mut Def,
    len: usize,
    cap: usize,
}

impl DefList {
    pub const fn empty() -> Self {
        Self {
            items: core::ptr::null_mut(),
            len: 0,
            cap: 0,
        }
    }

    pub fn as_slice(&self) -> &[Def] {
        if self.items.is_null() || self.len == 0 {
            &[]
        } else {
            unsafe { core::slice::from_raw_parts(self.items, self.len) }
        }
    }

    pub fn push(&mut self, def: Def) {
        if self.len == self.cap {
            let new_cap = if self.cap == 0 { 8 } else { self.cap * 2 };
            let new_size = new_cap * core::mem::size_of::<Def>();
            let new_ptr = unsafe {
                if self.items.is_null() {
                    malloc(new_size)
                } else {
                    realloc(self.items.cast(), new_size)
                }
            };
            if new_ptr.is_null() {
                return;
            }
            self.items = new_ptr.cast();
            self.cap = new_cap;
        }
        unsafe {
            *self.items.add(self.len) = def;
        }
        self.len += 1;
    }

    pub fn insert(&mut self, index: usize, def: Def) {
        if index >= self.len {
            self.push(def);
            return;
        }
        self.push(def);
        unsafe {
            let mut i = self.len - 1;
            while i > index {
                *self.items.add(i) = *self.items.add(i - 1);
                i -= 1;
            }
            *self.items.add(index) = def;
        }
    }

    pub fn remove_after(&mut self, start: usize, sid: Sid, tag: Tag) -> bool {
        let mut i = start;
        while i < self.len {
            let current = unsafe { *self.items.add(i) };
            if same(current.sid, sid) || same(current.tag, tag) {
                unsafe {
                    let mut j = i;
                    while j + 1 < self.len {
                        *self.items.add(j) = *self.items.add(j + 1);
                        j += 1;
                    }
                }
                self.len -= 1;
                return true;
            }
            i += 1;
        }
        false
    }
}

fn same(left: *const core::ffi::c_char, right: *const core::ffi::c_char) -> bool {
    if left.is_null() || right.is_null() {
        return false;
    }
    let mut i = 0;
    unsafe {
        while *left.add(i) != 0 && *right.add(i) != 0 {
            if *left.add(i) != *right.add(i) {
                return false;
            }
            i += 1;
        }
        *left.add(i) == 0 && *right.add(i) == 0
    }
}

pub static mut DEFS: DefList = DefList::empty();
pub static mut DID: bool = false;

pub fn with() -> Defs {
    unsafe {
        let did = *core::ptr::addr_of!(DID);
        let defs = &mut *core::ptr::addr_of_mut!(DEFS);
        if !did {
            for def in crate::impl_mod::all() {
                defs.push(*def);
            }
            *core::ptr::addr_of_mut!(DID) = true;
        }
        defs.as_slice()
    }
}

pub fn more(def: Def) -> i32 {
    unsafe {
        with();
        let defs = &mut *core::ptr::addr_of_mut!(DEFS);
        defs.insert(BUILD, def);
        0
    }
}

pub fn less(def: Def) -> i32 {
    unsafe {
        with();
        let defs = &mut *core::ptr::addr_of_mut!(DEFS);
        if defs.remove_after(BUILD, def.sid, def.tag) {
            0
        } else {
            -1
        }
    }
}
